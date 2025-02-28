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

#ifndef HAL_FILE_PC_HPP
#define HAL_FILE_PC_HPP

#include <fstream>
#include <ios>
#include <filesystem>
#include "HAL/File_Base.hpp"
#include "TemplateLibrary/String.hpp"

namespace VN
{

namespace Filesystem
{
inline bool exists(const FilePath& filePath) noexcept { return std::filesystem::exists(filePath.c_str()); }
}  // namespace Filesystem

inline Filesystem::FilePath operator/(Filesystem::FilePath lhs, const char* rhs) { return Filesystem::FilePath(lhs + "/" + rhs); }

class InputFile : public InputFile_Base
{
public:
    InputFile(const bool nullTerminateRead = false) : _nullTerminateRead(nullTerminateRead) {};
    InputFile(const Filesystem::FilePath& filePath, const bool nullTerminateRead = false)
        : _file(filePath, std::ios::binary), _nullTerminateRead(nullTerminateRead) {};

    virtual ~InputFile() {};

    InputFile& operator=(InputFile&& rhs) noexcept
    {
        if (this != &rhs) { _file = std::move(rhs._file); }
        return *this;
    }

    virtual bool open(const Filesystem::FilePath& filePath) override final
    {
        if (!_file.is_open() && !std::filesystem::exists(filePath.c_str())) { return true; }

        _file.open(filePath.c_str(), std::ios_base::binary);
        return !_file.good();
    }

    virtual void close() override final { _file.close(); };

    virtual bool read(char* buffer, const size_t count) override final
    {
        if (!_file.is_open() || !_file.good())
        {
            buffer[0] = '\0';
            return true;
        };
        if (_nullTerminateRead)
        {
            _file.read(buffer, count - 1);
            buffer[_file.gcount()] = '\0';
        }
        else { _file.read(buffer, count); }
        return !_file.good();
    }

    virtual bool getLine(char* buffer, const size_t capacity) override final
    {
        if (!_file.is_open() || !_file.good())
        {
            buffer[0] = '\0';
            return true;
        };
        _file.getline(buffer, capacity, '\n');

        // To make it cross-compatible, strip off \r if it is ending the file
        if (buffer[_file.gcount() - 2] == '\r') { buffer[_file.gcount() - 2] = '\0'; }

        return !_file.good();
    }

    virtual bool is_open() const override final { return _file.is_open(); };

    virtual void reset() override final
    {
        _file.clear();
        _file.seekg(0, std::ios::beg);
    }

private:
    std::ifstream _file;
    bool _nullTerminateRead;
};

class OutputFile : public OutputFile_Base
{
public:
    OutputFile() {};
    OutputFile(const Filesystem::FilePath& filePath) : _file(filePath, std::ios::binary) {};

    virtual ~OutputFile() {};

    OutputFile(const OutputFile&) = delete;
    OutputFile operator=(const OutputFile&) = delete;

    OutputFile(OutputFile&& rhs) : _file(std::move(rhs._file)) {}

    OutputFile& operator=(OutputFile&& rhs) noexcept
    {
        if (this != &rhs) { _file = std::move(rhs._file); }
        return *this;
    }

    virtual bool open(const Filesystem::FilePath& filePath) override final
    {
        _file.open(filePath.c_str(), std::ios_base::binary | std::ios_base::trunc);
        return !_file.good();
    }

    virtual void close() override final { _file.close(); };

    virtual bool write(const char* buffer, const size_t count) override final
    {
        if (!_file.is_open() && !_file.good()) { return true; };
        _file.write(buffer, count);
        return !_file.good();
    }

    virtual bool write(const char* buffer) override final { return write(buffer, std::strlen(buffer)); }

    virtual bool writeLine(const char* buffer, const size_t count) override final
    {
        if (write(buffer, count)) { return true; }
        return write("\n", 1);
    }

    virtual bool writeLine(const char* buffer) override final { return writeLine(buffer, std::strlen(buffer)); }

    virtual bool is_open() const override final { return _file.is_open(); }

    virtual void reset() override final
    {
        _file.clear();
        _file.seekp(0, std::ios::beg);
    }

private:
    std::ofstream _file;
};

}  // namespace VN

#endif  // HAL_FILE_PC_HPP
