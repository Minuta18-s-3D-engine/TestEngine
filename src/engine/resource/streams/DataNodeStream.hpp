#ifndef ENGINE_RESOURCE_STREAMS_DATANODESTREAM_HPP
#define ENGINE_RESOURCE_STREAMS_DATANODESTREAM_HPP

#include <stack>
#include "SerializationStream.hpp"
#include "engine/containers/structuredData/DataNode.hpp"

class DataNodeOutputStream : public SerializationOutputStream {
    StructuredData::DataNode root;
    std::stack<StructuredData::DataNode*> nodeStack;
    std::string currentKey;
public:
    explicit DataNodeOutputStream();

    void writeKey(std::string_view key) override;

    void writeBool(bool value) override;
    void writeInteger(int64_t value) override;
    void writeNumber(double value) override;
    void writeString(std::string_view value) override;
    void writeBytes(const void *data, size_t size) override;

    void writeBeginObject(const std::string &name) override;
    void writeEndObject() override;
    void writeBeginArray(const std::string &name) override;
    void writeEndArray() override;
};

class DataNodeInputStream : public SerializationInputStream {

public:
    explicit DataNodeInputStream();

    void readKey(std::string &key) override;

    void readBool(bool &value) override;
    void readInteger(int64_t &value) override;
    void readNumber(double &value) override;
    void readString(std::string &value) override;
    void readBytes(void *data, size_t size) override;

    void readBeginObject(std::string &name) override;
    void readEndObject() override;
    void readBeginArray(std::string &name) override;
    void readEndArray() override;
};

#endif // ENGINE_RESOURCE_STREAMS_DATANODESTREAM_HPP
