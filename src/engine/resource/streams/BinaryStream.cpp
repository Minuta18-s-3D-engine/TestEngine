#include "BinaryStream.hpp"

#include <cstring>

BinaryOutputStream::BinaryOutputStream(std::unique_ptr<File> binaryFile_)
    : binaryFile(std::move(binaryFile_)) {}

void BinaryOutputStream::writeBool(const bool value) {
    binaryFile->writeBytes(&value, sizeof(bool));
}

void BinaryOutputStream::writeInteger(const int64_t value) {
    binaryFile->writeBytes(&value, sizeof(int64_t));
}

void BinaryOutputStream::writeNumber(const double value) {
    binaryFile->writeBytes(&value, sizeof(double));
}

void BinaryOutputStream::writeString(const std::string_view value) {
    auto size = static_cast<uint32_t>(value.size());
    binaryFile->writeBytes(&size, sizeof(size));
    binaryFile->writeBytes(value.data(), value.size());
}

void BinaryOutputStream::writeBytes(const void* data, const size_t size) {
    binaryFile->writeBytes(data, size);
}

BinaryInputStream::BinaryInputStream(std::unique_ptr<File> binaryFile_)
    : binaryFile(std::move(binaryFile_)) {}

void BinaryInputStream::readBool(bool &value) {
    const auto bytes = binaryFile->readBytes(sizeof(bool));
    if (!bytes.empty()) {
        value = *reinterpret_cast<const bool*>(bytes.data());
    }
}

void BinaryInputStream::readInteger(int64_t &value) {
    const auto bytes = binaryFile->readBytes(sizeof(int64_t));
    if (bytes.size() == sizeof(int64_t)) {
        value = *reinterpret_cast<const int64_t*>(bytes.data());
    }
}

void BinaryInputStream::readNumber(double &value) {
    const auto bytes = binaryFile->readBytes(sizeof(double));
    if (bytes.size() == sizeof(double)) {
        value = *reinterpret_cast<const double*>(bytes.data());
    }
}

void BinaryInputStream::readString(std::string &value) {
   const auto sizeBytes = binaryFile->readBytes(sizeof(uint32_t));
    if (sizeBytes.size() < sizeof(uint32_t)) return;

    uint32_t size = *reinterpret_cast<const uint32_t*>(sizeBytes.data());
    auto dataBytes = binaryFile->readBytes(size);

    value.assign(
        reinterpret_cast<const char*>(dataBytes.data()), dataBytes.size()
    );
}

void BinaryInputStream::readBytes(void *data, const size_t size) {
    const auto bytes = binaryFile->readBytes(size);
    if (!bytes.empty() && data) {
        std::memcpy(data, bytes.data(), std::min(size, bytes.size()));
    }
}
