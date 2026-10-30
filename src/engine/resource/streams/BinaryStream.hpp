#ifndef ENGINE_RESOURCE_STREAMS_BINARYSTREAM_HPP
#define ENGINE_RESOURCE_STREAMS_BINARYSTREAM_HPP

#include "SerializationStream.hpp"
#include "engine/project/File.hpp"

class BinaryOutputStream : public SerializationOutputStream {
    std::unique_ptr<File> binaryFile;
public:
    explicit BinaryOutputStream(std::unique_ptr<File> binaryFile_);

    // BinaryStream implies that user has deterministic order of fields, so
    // keys, object names and array names can be safely omitted.
    void writeKey(std::string_view key) override {};

    void writeBool(bool value) override;
    void writeInteger(int64_t value) override;
    void writeNumber(double value) override;
    void writeString(std::string_view value) override;
    void writeBytes(const void *data, size_t size) override;

    void writeBeginObject(const std::string &name) override {};
    void writeEndObject() override {};
    void writeBeginArray(const std::string &name) override {};
    void writeEndArray() override {};
};

class BinaryInputStream : public SerializationInputStream {
    std::unique_ptr<File> binaryFile;
public:
    explicit BinaryInputStream(std::unique_ptr<File> binaryFile_);

    void readKey(std::string &key) override {};

    void readBool(bool &value) override;
    void readInteger(int64_t &value) override;
    void readNumber(double &value) override;
    void readString(std::string &value) override;
    void readBytes(void *data, size_t size) override;

    void readBeginObject(std::string &name) override {};
    void readEndObject() override {};
    void readBeginArray(std::string &name) override {};
    void readEndArray() override {};
};

#endif // ENGINE_RESOURCE_STREAMS_BINARYSTREAM_HPP
