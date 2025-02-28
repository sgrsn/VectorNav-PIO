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

#ifndef DATAEXPORT_EXPORTER_HPP
#define DATAEXPORT_EXPORTER_HPP

#include <atomic>
#include <cstdio>
#include <HAL/Thread.hpp>

#if THREADING_ENABLE
#include "HAL/Thread.hpp"
#endif
#include "Implementation/QueueDefinitions.hpp"

namespace VN
{

class Exporter
{
public:
    Exporter(const size_t& packetCapacity) : _queue{packetCapacity} {}

    virtual ~Exporter() = default;

    virtual void exportToFile() = 0;

#if THREADING_ENABLE
    bool start()
    {
        if (_thread == nullptr) { _logging = true; }
        else { return true; }
        _thread = std::make_unique<Thread>(&Exporter::_export, this);
        return false;
    }

    void stop()
    {
        _logging = false;
        _thread->join();
    }

    bool isLogging() const { return _logging; }
#endif

    PacketQueue_Interface* getQueuePtr() { return &_queue; }

protected:
    PacketQueue<1000> _queue;
#if THREADING_ENABLE
    std::atomic<bool> _logging = false;
    std::unique_ptr<Thread> _thread = nullptr;

private:
    void _export()
    {
        while (_logging)
        {
            thisThread::sleepFor(1ms);
            exportToFile();
        }
        exportToFile();
    }
#endif
};

}  // namespace VN

#endif  // DATAEXPORT_EXPORTER_HPP
