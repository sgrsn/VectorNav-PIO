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

#ifndef SIMPLELOGGER_HPP
#define SIMPLELOGGER_HPP

#include <cstdint>

#include "Interface/Sensor.hpp"
#include "HAL/File.hpp"
#include "TemplateLibrary/ByteBuffer.hpp"

namespace VN
{

class SimpleLogger
{
public:
    SimpleLogger(ByteBuffer& bufferToLog, const Filesystem::FilePath& filePath) : _bufferToLog(bufferToLog) { _logFile.open(filePath); }

    ~SimpleLogger() { stop(); }

    SimpleLogger(const SimpleLogger&) = delete;
    SimpleLogger& operator=(const SimpleLogger&) = delete;
    SimpleLogger(SimpleLogger&&) = delete;
    SimpleLogger& operator=(SimpleLogger&&) = delete;

    static int32_t logBuffer(OutputFile& outputFile, ByteBuffer& buffer)
    {
        if (!outputFile.is_open()) { return -1; }
        const size_t bufferSize = buffer.size();
        if (bufferSize == 0) { return 0; }
        size_t numLinearBytes = buffer.numLinearBytes();

        const size_t bytesToLog = std::min(bufferSize, numLinearBytes);
        if (outputFile.write(reinterpret_cast<const char*>(buffer.head()), bytesToLog)) { return -1; }
        size_t bytesLogged = bytesToLog;
        buffer.discard(bytesLogged);
        if (bufferSize > numLinearBytes)
        {
            numLinearBytes = buffer.numLinearBytes();
            bytesLogged += numLinearBytes;
            if (outputFile.write(reinterpret_cast<const char*>(buffer.head()), numLinearBytes)) { return -1; }  // Write the second section
            buffer.discard(numLinearBytes);
        }
        return bytesLogged;
    }

    bool start()
    {
        if (!_logFile.is_open()) { return true; }
        _logging = true;
        _loggingThread = std::make_unique<Thread>(&SimpleLogger::_log, this);
        return false;
    }

    void stop()
    {
        if (_logging)
        {
            _logging = false;
            _loggingThread->join();
        }
    }

    bool isLogging() { return _logging; }

    size_t numBytesLogged() { return _numBytesLogged; }

protected:
    void _log()
    {
        while (_logging)
        {
            _numBytesLogged += logBuffer(_logFile, _bufferToLog);
            thisThread::sleepFor(sleepDuration);
        }
        _numBytesLogged += logBuffer(_logFile, _bufferToLog);
        _logFile.close();
    }

    Microseconds sleepDuration = 1ms;
    std::atomic<bool> _logging = false;
    OutputFile _logFile;
    ByteBuffer& _bufferToLog;
    std::unique_ptr<Thread> _loggingThread = nullptr;
    size_t _numBytesLogged = 0;
};

}  // namespace VN
#endif  // SIMPLELOGGER_HPP
