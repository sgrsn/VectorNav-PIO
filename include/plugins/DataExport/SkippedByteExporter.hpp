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

#ifndef DATAEXPORT_SKIPPEDBYTEEXPORTER_HPP
#define DATAEXPORT_SKIPPEDBYTEEXPORTER_HPP

#include <atomic>
#include <cstdio>
#include <HAL/Thread.hpp>

#if THREADING_ENABLE
#include "HAL/Thread.hpp"
#endif
#include "HAL/File.hpp"
#include "TemplateLibrary/ByteBuffer.hpp"
#include "Exporter.hpp"

namespace VN
{

constexpr size_t SKIPPED_BYTE_BUFFER_CAPACITY = 1024 * 5;

class SkippedByteExporter
{
public:
    SkippedByteExporter(const Filesystem::FilePath& outputDir, const size_t& byteCapacity = SKIPPED_BYTE_BUFFER_CAPACITY)
        : _filePath{outputDir}, _queue{byteCapacity}
    {
        if (!_filePath.empty() && _filePath.back() != std::filesystem::path::preferred_separator)
        {
            _filePath = _filePath + std::filesystem::path::preferred_separator;
        }
    }

    ~SkippedByteExporter() = default;

    void exportToFile()
    {
        std::array<uint8_t, 1024 * 20> tmpBuffer;
        while (!_queue.isEmpty())
        {
            size_t bytes_to_get = std::min(tmpBuffer.size(), _queue.size());
            _queue.get(tmpBuffer.data(), bytes_to_get);
            _file.write(reinterpret_cast<const char*>(tmpBuffer.data()), bytes_to_get);
        }
    }

#if THREADING_ENABLE
    bool start()
    {
        if (_thread == nullptr) { _logging = true; }
        else { return true; }
        _thread = std::make_unique<Thread>(&SkippedByteExporter::_export, this);
        return false;
    }

    void stop()
    {
        _logging = false;
        _thread->join();
    }

    bool isLogging() const { return _logging; }
#endif

    ByteBuffer* getQueuePtr() { return &_queue; }

private:
    Filesystem::FilePath _filePath;
    OutputFile _file;
    ByteBuffer _queue;
#if THREADING_ENABLE
    std::atomic<bool> _logging = false;
    std::unique_ptr<Thread> _thread = nullptr;
#endif

    void _init_file()
    {
        Filesystem::FilePath fileName;
        std::snprintf(fileName.begin(), fileName.capacity(), "%sskippedBytes.bin", _filePath.c_str());
        _file = OutputFile(fileName);
    }

#if THREADING_ENABLE
    void _export()
    {
        _init_file();
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

#endif  // DATAEXPORT_SKIPPEDBYTEEXPORTER_HPP
