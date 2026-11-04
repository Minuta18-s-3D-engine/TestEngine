#include "BinaryStream.hpp"

#include <cstring>

BinaryOutputStream::BinaryOutputStream(std::unique_ptr<File> binaryFile_)
    : binaryFile(std::move(binaryFile_)) {}

void BinaryOutputStream::writeBool(const bool value) {
    binaryFile->writeBinary(&value, sizeof(bool));
}

void BinaryOutputStream::writeInteger(const int64_t value) {
    binaryFile->writeBinary(&value, sizeof(int64_t));
}

void BinaryOutputStream::writeNumber(const double value) {
    binaryFile->writeBinary(&value, sizeof(double));
}

void BinaryOutputStream::writeString(const std::string_view value) {
    auto size = static_cast<uint32_t>(value.size());
    binaryFile->writeBinary(&size, sizeof(size));
    binaryFile->writeBinary(value.data(), value.size());
}

void BinaryOutputStream::writeBinary(const void* data, const size_t size) {
    binaryFile->writeBinary(data, size);
}

BinaryInputStream::BinaryInputStream(std::unique_ptr<File> binaryFile_)
    : binaryFile(std::move(binaryFile_)) {}

void BinaryInputStream::readBool(bool &value) {
    const auto binary = binaryFile->readBinary(sizeof(bool));
    if (!binary.empty()) {
        value = *reinterpret_cast<const bool*>(binary.data());
    }
}

void BinaryInputStream::readInteger(int64_t &value) {
    const auto binary = binaryFile->readBinary(sizeof(int64_t));
    if (binary.size() == sizeof(int64_t)) {
        value = *reinterpret_cast<const int64_t*>(binary.data());
    }
}

void BinaryInputStream::readNumber(double &value) {
    const auto binary = binaryFile->readBinary(sizeof(double));
    if (binary.size() == sizeof(double)) {
        value = *reinterpret_cast<const double*>(binary.data());
    }
}

void BinaryInputStream::readString(std::string &value) {
   const auto sizeBinary = binaryFile->readBinary(sizeof(uint32_t));
    if (sizeBinary.size() < sizeof(uint32_t)) return;

    uint32_t size = *reinterpret_cast<const uint32_t*>(sizeBinary.data());
    auto dataBinary = binaryFile->readBinary(size);

    value.assign(
        reinterpret_cast<const char*>(dataBinary.data()), dataBinary.size()
    );
}

void BinaryInputStream::readBinary(void *data, const size_t size) {
    const auto binary = binaryFile->readBinary(size);
    if (!binary.empty() && data) {
        std::memcpy(data, binary.data(), std::min(size, binary.size()));
    }
}
