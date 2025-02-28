// The MIT License (MIT)
// 
// Copyright (c) Hidaka SATO
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

#ifndef HAL_FILE_MBED_HPP
#define HAL_FILE_MBED_HPP

#include "HAL/File_Base.hpp"
#include "TemplateLibrary/String.hpp"

namespace VN
{

namespace Filesystem
{
inline bool exists(const FilePath& filePath) noexcept
{
    FILE* file = fopen(filePath.c_str(), "r");
    if (file != nullptr)
    {
        fclose(file);
        return true;
    }
    return false;
}
}  // namespace Filesystem

inline Filesystem::FilePath operator/(Filesystem::FilePath lhs, const char* rhs) { return Filesystem::FilePath(lhs + "/" + rhs); }

class InputFile : public InputFile_Base
{
public:
    InputFile(const bool nullTerminateRead = false) : _nullTerminateRead(nullTerminateRead), _file(nullptr) {};
    InputFile(const Filesystem::FilePath& filePath, const bool nullTerminateRead = false)
        : _nullTerminateRead(nullTerminateRead), _file(nullptr)
    {
        open(filePath);
    };

    virtual ~InputFile()
    {
        close();
    };

    InputFile& operator=(InputFile&& rhs) noexcept
    {
        if (this != &rhs)
        {
            close();
            _file = rhs._file;
            _nullTerminateRead = rhs._nullTerminateRead;
            rhs._file = nullptr;
        }
        return *this;
    }

    virtual bool open(const Filesystem::FilePath& filePath) override final
    {
        if (_file != nullptr)
        {
            close();
        }
        
        if (!Filesystem::exists(filePath))
        {
            return true;
        }

        _file = fopen(filePath.c_str(), "rb");
        return (_file == nullptr);
    }

    virtual void close() override final
    {
        if (_file != nullptr)
        {
            fclose(_file);
            _file = nullptr;
        }
    };

    virtual bool read(char* buffer, const size_t count) override final
    {
        if (_file == nullptr)
        {
            buffer[0] = '\0';
            return true;
        }
        
        size_t bytesRead = 0;
        if (_nullTerminateRead)
        {
            bytesRead = fread(buffer, 1, count - 1, _file);
            buffer[bytesRead] = '\0';
        }
        else
        {
            bytesRead = fread(buffer, 1, count, _file);
        }
        
        return (bytesRead != count);
    }

    virtual bool getLine(char* buffer, const size_t capacity) override final
    {
        if (_file == nullptr)
        {
            buffer[0] = '\0';
            return true;
        }
        
        if (fgets(buffer, capacity, _file) == nullptr)
        {
            buffer[0] = '\0';
            return true;
        }
        
        // Remove newline if present
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n')
        {
            buffer[len - 1] = '\0';
            len--;
        }
        
        // To make it cross-compatible, strip off \r if it is ending the line
        if (len > 0 && buffer[len - 1] == '\r')
        {
            buffer[len - 1] = '\0';
        }
        
        return false;
    }

    virtual bool is_open() const override final { return (_file != nullptr); };

    virtual void reset() override final
    {
        if (_file != nullptr)
        {
            fseek(_file, 0, SEEK_SET);
        }
    }

private:
    FILE* _file;
    bool _nullTerminateRead;
};

class OutputFile : public OutputFile_Base
{
public:
    OutputFile() : _file(nullptr) {};
    OutputFile(const Filesystem::FilePath& filePath) : _file(nullptr)
    {
        open(filePath);
    };

    virtual ~OutputFile()
    {
        close();
    };

    OutputFile(const OutputFile&) = delete;
    OutputFile operator=(const OutputFile&) = delete;

    OutputFile(OutputFile&& rhs) : _file(rhs._file)
    {
        rhs._file = nullptr;
    }

    OutputFile& operator=(OutputFile&& rhs) noexcept
    {
        if (this != &rhs)
        {
            close();
            _file = rhs._file;
            rhs._file = nullptr;
        }
        return *this;
    }

    virtual bool open(const Filesystem::FilePath& filePath) override final
    {
        if (_file != nullptr)
        {
            close();
        }
        
        _file = fopen(filePath.c_str(), "wb");
        return (_file == nullptr);
    }

    virtual void close() override final
    {
        if (_file != nullptr)
        {
            fclose(_file);
            _file = nullptr;
        }
    };

    virtual bool write(const char* buffer, const size_t count) override final
    {
        if (_file == nullptr)
        {
            return true;
        }
        
        size_t bytesWritten = fwrite(buffer, 1, count, _file);
        return (bytesWritten != count);
    }

    virtual bool write(const char* buffer) override final
    {
        return write(buffer, strlen(buffer));
    }

    virtual bool writeLine(const char* buffer, const size_t count) override final
    {
        if (write(buffer, count))
        {
            return true;
        }
        return write("\n", 1);
    }

    virtual bool writeLine(const char* buffer) override final
    {
        return writeLine(buffer, strlen(buffer));
    }

    virtual bool is_open() const override final
    {
        return (_file != nullptr);
    }

    virtual void reset() override final
    {
        if (_file != nullptr)
        {
            fseek(_file, 0, SEEK_SET);
        }
    }

private:
    FILE* _file;
};

}  // namespace VN

#endif  // HAL_FILE_MBED_HPP