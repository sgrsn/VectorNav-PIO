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

#ifndef DATAEXPORT_EXPORTERASCII_HPP
#define DATAEXPORT_EXPORTERASCII_HPP

#include "Exporter.hpp"
#include "HAL/File.hpp"
#include "Implementation/Packet.hpp"
#include "Interface/Command.hpp"
#include "Debug.hpp"

namespace VN
{

class ExporterAscii : public Exporter
{
private:
    static constexpr uint8_t MAX_NUM_FILES = 10;
    static constexpr uint16_t EXPORTER_PACKET_CAPACITY = 2048;

public:
    ExporterAscii(const Filesystem::FilePath& outputDir, bool enableSystemTimeStamps = false)
        : Exporter(EXPORTER_PACKET_CAPACITY), _filePath(outputDir), _enableSystemTimeStamps(enableSystemTimeStamps)
    {
        if (!_filePath.empty() && _filePath.to_string().back() != std::filesystem::path::preferred_separator)
        {
            _filePath = _filePath + std::filesystem::path::preferred_separator;
        }
    }

    ~ExporterAscii() {}

    struct AsciiInfo
    {
        AsciiHeader header;
        OutputFile file;
    };

    void exportToFile() override
    {
        while (!_queue.isEmpty())
        {
            const auto p = _queue.get();
            if (!p || (p->details.syncByte != PacketDetails::SyncByte::Ascii)) { continue; }

            if (_asciiInfo.size() == _asciiInfo.capacity())
            {
                VN_DEBUG_1("Packet dropped.");
                continue;
            }

            OutputFile& ascii = getFileHandle(p->details.asciiMetadata.header);

            if (_enableSystemTimeStamps)
            {
                char _buffer[32];
                const auto len = std::snprintf(
                    _buffer, sizeof(_buffer), "%lld:",
                    static_cast<long long int>(std::chrono::duration_cast<Nanoseconds>(p->details.asciiMetadata.timestamp.time_since_epoch()).count()));
                ascii.write(_buffer, len);
            }

            ascii.write(reinterpret_cast<const char*>(p->buffer), p->details.asciiMetadata.length);
        }
    }

private:
    Filesystem::FilePath _filePath;
    const bool _enableSystemTimeStamps = false;

    Vector<AsciiInfo, MAX_NUM_FILES> _asciiInfo;  // One entry per file created, which is per unique message

    OutputFile& getFileHandle(const AsciiHeader& header)
    {
        for (auto& tmp : _asciiInfo)
        {
            if (tmp.header == header) { return tmp.file; }
        }

        // if we don't find the header we need to init a new text file
        AsciiMessage fileName = _filePath + header + ".txt";

        // The size of this has already been checked
        if (_asciiInfo.push_back(AsciiInfo{header, OutputFile(fileName)})) { VN_ABORT(); }

        return _asciiInfo.back().file;
    }
};

}  // namespace VN

#endif  // DATAEXPORT_EXPORTERASCII_HPP
