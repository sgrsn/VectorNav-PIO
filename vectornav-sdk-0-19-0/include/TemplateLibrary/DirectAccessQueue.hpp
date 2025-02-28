// The MIT License (MIT)
// 
// VectorNav SDK (v0.19.0)
// Copyright (c) 2024 VectorNav Technologies, LLC
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef TEMPLATELIBRARY_DIRECTACCESSQUEUE_HPP
#define TEMPLATELIBRARY_DIRECTACCESSQUEUE_HPP

#include <array>
#include <memory>
#include <cstdint>
#include "HAL/Mutex.hpp"
#include "TemplateLibrary/Queue.hpp"

namespace VN
{

template <class Type, std::size_t... Is>
constexpr std::array<Type, sizeof...(Is)> initializeArray(int arg, std::index_sequence<Is...>)
{
    return {{(static_cast<void>(Is), Type(arg))...}};  // cast removes unused parameter warning
}

template <class ItemType>
class DirectAccessQueue_Interface
{
public:
    struct Element
    {
        ItemType item;
        enum class Status
        {
            Free,
            Putting,
            Getting,
            InQueue
        };
        std::atomic<Status> status = Status::Free;

        template <typename... ConstructArgs>
        Element(ConstructArgs&&... args) : item(std::forward<ConstructArgs>(args)...)
        {
        }

        Element(Element&& other) = delete;
        Element(const Element& other) = delete;
        Element& operator=(Element&& other) = delete;
        Element& operator=(const Element& other) = delete;
    };

    class OwningPtr
    {
    public:
        OwningPtr() = default;
        OwningPtr(DirectAccessQueue_Interface::Element* element) : _element(element) {};
        ~OwningPtr() { _clearElementStatus(); }

        OwningPtr(OwningPtr&& other) noexcept : _element(other._element) { other._element = nullptr; }

        OwningPtr& operator=(OwningPtr&& other) noexcept
        {
            if (this != &other)
            {
                _clearElementStatus();
                _element = other._element;
                other._element = nullptr;
            }
            return *this;
        }

        OwningPtr(const OwningPtr& other) = delete;
        OwningPtr& operator=(const OwningPtr& other) = delete;

        OwningPtr(std::nullptr_t) : _element(nullptr) {}

        OwningPtr& operator=(std::nullptr_t)
        {
            _clearElementStatus();
            _element = nullptr;
            return *this;
        }

        bool operator==(std::nullptr_t) const { return _element == nullptr; }

        bool operator!=(std::nullptr_t) const { return _element != nullptr; }

        operator bool() const { return _element != nullptr; }

        ItemType* get() const noexcept { return &_element->item; }

        ItemType& operator*() { return _element->item; }
        const ItemType& operator*() const { return _element->item; }

        ItemType* operator->() { return &_element->item; }
        const ItemType* operator->() const { return &_element->item; }

    private:
        void _clearElementStatus()
        {
            if (_element)
            {
                if (_element->status == Element::Status::Getting) { _element->status = Element::Status::Free; }
                else if (_element->status == Element::Status::Putting) { _element->status = Element::Status::InQueue; }
            }
        }
        DirectAccessQueue_Interface::Element* _element = nullptr;
    };

    using value_type = OwningPtr;  // Used to be able to arbitrate away implementation in Sensor
    virtual OwningPtr put() noexcept = 0;
    virtual OwningPtr get() noexcept = 0;
    virtual OwningPtr getBack() noexcept = 0;
    virtual void reset() noexcept = 0;
    virtual uint16_t size() const noexcept = 0;
    virtual bool isEmpty() const noexcept = 0;
    virtual uint16_t capacity() const noexcept = 0;
};

template <class ItemType, size_t Capacity>
class DirectAccessQueue : public DirectAccessQueue_Interface<ItemType>
{
public:
    using OwningPtr = typename DirectAccessQueue_Interface<ItemType>::OwningPtr;
    using Element = typename DirectAccessQueue_Interface<ItemType>::Element;

    template <typename... Args>
    DirectAccessQueue(Args&&... args) : _elements{std::forward<Args>(args)...}
    {
    }

    // Used for array initialization of a single value
    template <class CArg>
    DirectAccessQueue(CArg&& arg) : _elements(initializeArray<Element>(arg, std::make_index_sequence<Capacity>{}))
    {
    }

    DirectAccessQueue(DirectAccessQueue&& other) = delete;
    DirectAccessQueue(const DirectAccessQueue& other) = delete;
    DirectAccessQueue& operator=(DirectAccessQueue&& other) = delete;
    DirectAccessQueue& operator=(const DirectAccessQueue& other) = delete;

    virtual OwningPtr put() noexcept override final
    {
        LockGuard lock(_mutex);
        uint16_t i = 0;
        for (auto& element : _elements)
        {
            if (element.status == Element::Status::Free)
            {
                element.status = Element::Status::Putting;
                _circularBuffer.put(i);
                return &element;
            }
            ++i;
        }

        // Queue is totally full. We can go to the queue head and overwrite it, force pushing.
        _reset();
        // Retry the exact same thing
        i = 0;
        for (auto& element : _elements)
        {
            if (element.status == Element::Status::Free)
            {
                element.status = Element::Status::Putting;
                _circularBuffer.put(i);
                return &element;
            }
            ++i;
        }
        VN_DEBUG_2("Request put failed.");
        return nullptr;
    }

    virtual void reset() noexcept override final
    {
        LockGuard mutex(_mutex);
        _reset();
    }

    virtual OwningPtr get() noexcept override final
    {
        LockGuard lock(_mutex);
        auto nextIdx = _circularBuffer.peek();
        if (!nextIdx.has_value()) { return nullptr; }
        if (_elements[nextIdx.value()].status != Element::Status::InQueue)
        {
            // Item is still being put
            return nullptr;
        }

        nextIdx = _circularBuffer.get();  // Actually pop it from the queue
        _elements[*nextIdx].status = Element::Status::Getting;
        return &_elements[*nextIdx];
    }

    virtual OwningPtr getBack() noexcept override final
    {
        LockGuard lock(_mutex);
        uint16_t latestIdx;
        bool found = false;
        while (true)
        {
            auto nextIdx = _circularBuffer.peek();
            if (!nextIdx || (_elements[*nextIdx].status != Element::Status::InQueue)) { break; }
            _elements[*nextIdx].status = Element::Status::Free;
            _circularBuffer.get();
            latestIdx = *nextIdx;
            found = true;
        }
        if (!found) { return nullptr; }
        _elements[latestIdx].status = Element::Status::Getting;
        return &_elements[latestIdx];
    }

    virtual uint16_t size() const noexcept override final
    {
        LockGuard lock(_mutex);
        uint16_t queueSize = _circularBuffer.size();

        for (const auto& element : _elements)
        {
            if (element.status == Element::Status::Putting) { --queueSize; }
        }
        return queueSize;
    }

    virtual bool isEmpty() const noexcept override final { return _circularBuffer.isEmpty() || (size() == 0); }

    virtual uint16_t capacity() const noexcept override final { return Capacity; }

private:
    std::array<Element, Capacity> _elements;
    Queue<uint16_t, Capacity> _circularBuffer;
    mutable Mutex _mutex;

    void _reset() noexcept
    {
        while (true)
        {
            auto nextIdx = _circularBuffer.peek();
            if (nextIdx.has_value() && (_elements[*nextIdx].status == Element::Status::InQueue))
            {
                _circularBuffer.get();  // Pop it from queue
                _elements[*nextIdx].status = Element::Status::Free;
            }
            else
            {
                break;  // Assume the rest are "putting", and don't clear any further items. Or buffer is empty and we've cleared them all.
            }
        }
    }
};

}  // namespace VN

#endif  // TEMPLATELIBRARY_DIRECTACCESSQUEUE_HPP
