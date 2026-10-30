#ifndef ENGINE_RESOURCE_STREAMS_SERIALIZATIONSTREAM_HPP
#define ENGINE_RESOURCE_STREAMS_SERIALIZATIONSTREAM_HPP

#include <cstdint>
#include <string_view>

class SerializationOutputStream {
public:
    virtual ~SerializationOutputStream() = default;

    virtual void writeKey(std::string_view key) = 0;

    virtual void writeBool(bool value) = 0;
    virtual void writeInteger(int64_t value) = 0;
    virtual void writeNumber(double value) = 0;
    virtual void writeString(std::string_view value) = 0;
    virtual void writeBytes(const void* data, size_t size) = 0;

    virtual void writeBeginObject(const std::string& name) = 0;
    virtual void writeEndObject() = 0;
    virtual void writeBeginArray(const std::string& name) = 0;
    virtual void writeEndArray() = 0;
};

class SerializationInputStream {
public:
    virtual ~SerializationInputStream() = default;

    virtual void readKey(std::string& key) = 0;

    virtual void readBool(bool& value) = 0;
    virtual void readInteger(int64_t& value) = 0;
    virtual void readNumber(double& value) = 0;
    virtual void readString(std::string& value) = 0;
    virtual void readBytes(void* data, size_t size) = 0;

    virtual void readBeginObject(std::string& name) = 0;
    virtual void readEndObject() = 0;
    virtual void readBeginArray(std::string& name) = 0;
    virtual void readEndArray() = 0;
};

#endif // ENGINE_RESOURCE_STREAMS_SERIALIZATIONSTREAM_HPP
