#ifndef ENGINE_RESOURCE_STREAMS_DATANODESTREAM_HPP
#define ENGINE_RESOURCE_STREAMS_DATANODESTREAM_HPP

#include <stack>
#include "SerializationStream.hpp"
#include "engine/containers/structuredData/DataNode.hpp"

class DataNodeOutputStream : public SerializationOutputStream {
    StructuredData::DataNode root;
    std::stack<StructuredData::DataNode*> nodeStack;
    std::string currentKey;

    void insertNode(StructuredData::DataNode node);
public:
    explicit DataNodeOutputStream();

    void writeKey(std::string_view key) override;

    void writeBool(bool value) override;
    void writeInteger(int64_t value) override;
    void writeNumber(double value) override;
    void writeString(std::string_view value) override;
    void writeBinary(const void *data, size_t size) override;

    void writeBeginObject() override;
    void writeEndObject() override;
    void writeBeginArray() override;
    void writeEndArray() override;

    [[nodiscard]] StructuredData::DataNode getResult();
};

class DataNodeInputStream : public SerializationInputStream {
    const StructuredData::DataNode& root;
    std::stack<const StructuredData::DataNode*> nodeStack;
    std::stack<typename StructuredData::Object::NodesMap::const_iterator>
        objectIterators;
    std::stack<size_t> arrayIndices;

    const StructuredData::DataNode* getNextNode();
public:
    explicit DataNodeInputStream(const StructuredData::DataNode& rootNode);

    void readKey(std::string &key) override;

    void readBool(bool &value) override;
    void readInteger(int64_t &value) override;
    void readNumber(double &value) override;
    void readString(std::string &value) override;
    void readBinary(void *data, size_t size) override;

    void readBeginObject() override;
    void readEndObject() override;
    void readBeginArray() override;
    void readEndArray() override;
};

#endif // ENGINE_RESOURCE_STREAMS_DATANODESTREAM_HPP
