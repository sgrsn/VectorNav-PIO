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

#ifndef DATAEXPORT_FILEEXPORTER_HPP
#define DATAEXPORT_FILEEXPORTER_HPP

#include <memory>
#include <filesystem>

#include "Exporter.hpp"
#include "SkippedByteExporter.hpp"
#include "HAL/File.hpp"
#include "TemplateLibrary/Vector.hpp"
#include "Interface/Registers.hpp"
#include "Implementation/PacketSynchronizer.hpp"
#include "Implementation/AsciiPacketDispatcher.hpp"
#include "Implementation/FaPacketDispatcher.hpp"
#include "Implementation/FbPacketDispatcher.hpp"

namespace VN
{

constexpr uint8_t EXPORTER_CAPACITY = 5;
constexpr uint8_t ASYNC_ERROR_QUEUE_CAPACITY = 5;

class FileExporter
{
public:
    FileExporter() = default;

    bool processFile(const Filesystem::FilePath& fileName)
    {
        const auto fileSizeInBytes = std::filesystem::file_size(fileName.c_str());
        auto buffer = std::make_unique<uint8_t[]>(fileSizeInBytes);

        InputFile inputFile(fileName);

        inputFile.read(reinterpret_cast<char*>(buffer.get()), fileSizeInBytes);

        ByteBuffer byteBuffer(buffer.get(), fileSizeInBytes, fileSizeInBytes);

        PacketSynchronizer packetSynchronizer(byteBuffer, [this](AsyncError&& error) { _asyncErrorQueue.put(std::move(error)); });
        packetSynchronizer.addDispatcher(&_asciiPacketDispatcher);
        packetSynchronizer.addDispatcher(&_faPacketDispatcher);
        packetSynchronizer.addDispatcher(&_fbPacketDispatcher);

        Registers::System::BinaryOutputMeasurements bor;

        for (auto& e : _exporters)
        {
            _faPacketDispatcher.addSubscriber(e->getQueuePtr(), bor.toBinaryHeader().toMeasurementHeader(), FaPacketDispatcher::SubscriberFilterType::AnyMatch);
            _asciiPacketDispatcher.addSubscriber(e->getQueuePtr(), "VN", AsciiPacketDispatcher::SubscriberFilterType::StartsWith);

            e->start();
        }
        packetSynchronizer.registerSkippedByteBuffer(_skippedByteExporter->getQueuePtr());
        if (_skippedByteExporter) { _skippedByteExporter->start(); }

        while (!packetSynchronizer.dispatchNextPacket()) {};

        for (auto& e : _exporters) { e->stop(); }
        if (_skippedByteExporter) { _skippedByteExporter->stop(); }
        
        _parsingStats.validFaPacketCount = packetSynchronizer.getValidPacketCount(PacketSynchronizer::SyncBytes{0xFA});
        _parsingStats.invalidFaPacketCount = packetSynchronizer.getInvalidPacketCount(PacketSynchronizer::SyncBytes{0xFA});
        _parsingStats.validAsciiPacketCount = packetSynchronizer.getValidPacketCount(PacketSynchronizer::SyncBytes{'$'});
        _parsingStats.invalidAsciiPacketCount = packetSynchronizer.getInvalidPacketCount(PacketSynchronizer::SyncBytes{'$'});
        _parsingStats.validFbPacketCount = packetSynchronizer.getValidPacketCount(PacketSynchronizer::SyncBytes{0xFB});
        _parsingStats.invalidFbPacketCount = packetSynchronizer.getInvalidPacketCount(PacketSynchronizer::SyncBytes{0xFB});
        _parsingStats.skippedByteCount = packetSynchronizer.getSkippedByteCount();
        _parsingStats.receivedByteCount = packetSynchronizer.getReceivedByteCount();
        return false;
    }

    bool addExporter(std::unique_ptr<Exporter>&& exporterToAdd) { return _exporters.push_back(std::move(exporterToAdd)); };
    void addSkippedByteExporter(std::unique_ptr<SkippedByteExporter>&& exporterToAdd) { _skippedByteExporter = std::move(exporterToAdd); }

    struct ParsingStats
    {
        uint64_t validFaPacketCount = 0;
        uint64_t invalidFaPacketCount = 0;
        uint64_t validAsciiPacketCount = 0;
        uint64_t invalidAsciiPacketCount = 0;
        uint64_t validFbPacketCount = 0;
        uint64_t invalidFbPacketCount = 0;
        uint64_t skippedByteCount = 0;
        uint64_t receivedByteCount = 0;
    };

    ParsingStats getParsingStats() { return _parsingStats; }

private:
    MeasurementQueue _measurementQueue{Config::PacketDispatchers::compositeDataQueueCapacity};

    CommandProcessor _commandProcessor{[]([[maybe_unused]] AsyncError&& error) {}};

    AsciiPacketDispatcher _asciiPacketDispatcher{&_measurementQueue, Config::PacketDispatchers::cdEnabledMeasTypes, &_commandProcessor};

    FaPacketDispatcher _faPacketDispatcher{&_measurementQueue, Config::PacketDispatchers::cdEnabledMeasTypes};
    FbPacketDispatcher _fbPacketDispatcher{&_faPacketDispatcher, Config::PacketFinders::fbBufferCapacity};

    Vector<std::unique_ptr<Exporter>, EXPORTER_CAPACITY> _exporters;
    std::unique_ptr<SkippedByteExporter> _skippedByteExporter = nullptr;
    Queue_Mutexed<AsyncError, ASYNC_ERROR_QUEUE_CAPACITY> _asyncErrorQueue;
    ParsingStats _parsingStats{};
};

std::ostream& operator<<(std::ostream& os, const FileExporter::ParsingStats& stats)
{
    uint64_t totalValidPackets = stats.validFaPacketCount + stats.validAsciiPacketCount + stats.validFbPacketCount;
    double validFaPercent = 100.0 * stats.validFaPacketCount / totalValidPackets;
    double validAsciiPercent = 100.0 * stats.validAsciiPacketCount / totalValidPackets;
    double validFbPercent = 100.0 * stats.validFbPacketCount / totalValidPackets;
    double skippedBytePercent = 100.0 * stats.skippedByteCount / stats.receivedByteCount;

    os << std::setw(26) << std::left << "Valid FA Packets: " << stats.validFaPacketCount << " (" << std::fixed << std::setprecision(2) << validFaPercent
       << "%)\n";
    os << std::setw(26) << std::left << "Invalid FA Packets: " << stats.invalidFaPacketCount << "\n";
    os << std::setw(26) << std::left << "Valid ASCII Packets: " << stats.validAsciiPacketCount << " (" << std::fixed << std::setprecision(2)
       << validAsciiPercent << "%)\n";
    os << std::setw(26) << std::left << "Invalid ASCII Packets: " << stats.invalidAsciiPacketCount << "\n";
    os << std::setw(26) << std::left << "Valid FB Packets: " << stats.validFbPacketCount << " (" << std::fixed << std::setprecision(2) << validFbPercent
       << "%)\n";
    os << std::setw(26) << std::left << "Invalid FB Packets: " << stats.invalidFbPacketCount << "\n";
    os << std::setw(26) << std::left << "Skipped Bytes: " << stats.skippedByteCount << " (" << std::fixed << std::setprecision(2) << skippedBytePercent
       << "%)\n";
    os << std::setw(26) << std::left << "Received Bytes: " << stats.receivedByteCount << "\n";
    os << std::setw(26) << std::left << "Total Valid Packet Count: " << totalValidPackets << "\n";
    os << std::setw(26) << std::left << "Overall Packet Count: "
       << stats.validFaPacketCount + stats.invalidFaPacketCount + stats.validAsciiPacketCount + stats.invalidAsciiPacketCount + stats.validFbPacketCount +
              stats.invalidFbPacketCount
       << "\n";

    return os;
}

}  // namespace VN

#endif  // DATAEXPORT_FILEEXPORTER_HPP
