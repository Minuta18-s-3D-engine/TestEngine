#include "DataNodeStream.hpp"

#include <cstring>

DataNodeOutputStream::DataNodeOutputStream() {
    root = StructuredData::DataNode(StructuredData::Object{});
    nodeStack.push(&root);
}

void DataNodeOutputStream::insertNode(StructuredData::DataNode node) {
    auto* topNode = nodeStack.top();
    if (topNode->isObject()) {
        topNode->asObject()[currentKey] = std::move(node);
    } else if (topNode->isArray()) {
        topNode->asArray().push_back(std::move(node));
    }
    currentKey.clear();
}

void DataNodeOutputStream::writeKey(std::string_view key) {
    currentKey = std::string(key);
}

void DataNodeOutputStream::writeBool(const bool value) {
    insertNode(StructuredData::DataNode(value));
}

void DataNodeOutputStream::writeInteger(const int64_t value) {
    insertNode(StructuredData::DataNode(value));
}

void DataNodeOutputStream::writeNumber(const double value) {
    insertNode(StructuredData::DataNode(value));
}

void DataNodeOutputStream::writeString(const std::string_view value) {
    insertNode(StructuredData::DataNode(std::string(value)));
}

void DataNodeOutputStream::writeBinary(const void *data, const size_t size) {
    const auto* bytePtr = static_cast<const uint8_t*>(data);
    std::vector<uint8_t> binary(bytePtr, bytePtr + size);
    insertNode(StructuredData::DataNode(std::move(binary)));
}

void DataNodeOutputStream::writeBeginObject() {
    StructuredData::DataNode objNode(StructuredData::Object{});

    auto* topNode = nodeStack.top();
    StructuredData::DataNode* childPtr = nullptr;

    if (topNode->isObject()) {
        topNode->asObject()[currentKey] = std::move(objNode);
        childPtr = &((*topNode)[currentKey]);
    } else if (topNode->isArray()) {
        topNode->asArray().push_back(std::move(objNode));
        childPtr = &(topNode->asArray().back());
    }

    nodeStack.push(childPtr);
    currentKey.clear();
}

void DataNodeOutputStream::writeEndObject() {
    if (nodeStack.size() > 1) {
        nodeStack.pop();
    }
}

void DataNodeOutputStream::writeBeginArray() {
    StructuredData::DataNode arrNode(StructuredData::Array{});

    auto* topNode = nodeStack.top();
    StructuredData::DataNode* childPtr = nullptr;
    if (topNode->isObject()) {
        topNode->asObject()[currentKey] = std::move(arrNode);
        childPtr = &((*topNode)[currentKey]);
    } else if (topNode->isArray()) {
        topNode->asArray().push_back(std::move(arrNode));
        childPtr = &(topNode->asArray().back());
    }

    nodeStack.push(childPtr);
    currentKey.clear();
}

void DataNodeOutputStream::writeEndArray() {
    if (nodeStack.size() > 1) {
        nodeStack.pop();
    }
}

StructuredData::DataNode DataNodeOutputStream::getResult() {
    return std::move(root);
}


DataNodeInputStream::DataNodeInputStream(
    const StructuredData::DataNode& rootNode
) : root(rootNode) {
    nodeStack.push(&root);
    if (root.isObject()) {
        objectIterators.push(root.asObject().begin());
    } else if (root.isArray()) {
        arrayIndices.push(0);
    }
}

const StructuredData::DataNode *DataNodeInputStream::getNextNode() {
    auto* topNode = nodeStack.top();
    if (topNode->isObject()) {
        if (objectIterators.empty()) return nullptr;
        auto& it = objectIterators.top();
        if (it == topNode->asObject().end()) return nullptr;
        const auto* node = &(it->second);
        ++it;
        return node;
    } else if (topNode->isArray()) {
        if (arrayIndices.empty()) return nullptr;
        size_t& idx = arrayIndices.top();
        const auto& arr = topNode->asArray();
        if (idx >= arr.size()) return nullptr;
        return &arr[idx++];
    }
    return nullptr;
}

void DataNodeInputStream::readKey(std::string &key) {
    if (!objectIterators.empty() && nodeStack.top()->isObject()) {
        auto it = objectIterators.top();
        if (it != nodeStack.top()->asObject().end()) {
            key = it->first;
        }
    }
}

void DataNodeInputStream::readBool(bool &value) {
    if (auto* node = getNextNode(); node && node->isBoolean()) {
        value = node->asBoolean();
    }
}

void DataNodeInputStream::readInteger(int64_t &value) {
    if (auto* node = getNextNode(); node && node->isInteger()) {
        value = node->asInteger();
    }
}

void DataNodeInputStream::readNumber(double &value) {
    if (auto* node = getNextNode(); node && node->isNumber()) {
        value = node->asNumber();
    }
}

void DataNodeInputStream::readString(std::string &value) {
    if (auto* node = getNextNode(); node && node->isString()) {
        value = node->asString();
    }
}

void DataNodeInputStream::readBinary(void *data, size_t size) {
    if (auto* node = getNextNode(); node && node->isBinary()) {
        const auto& binary = node->asBinary();
        std::memcpy(data, binary.data(), std::min(size, binary.size()));
    }
}

void DataNodeInputStream::readBeginObject() {
    if (auto* node = getNextNode(); node && node->isObject()) {
        nodeStack.push(node);
        objectIterators.push(node->asObject().begin());
    }
}

void DataNodeInputStream::readEndObject() {
    if (nodeStack.size() > 1) {
        nodeStack.pop();
        if (!objectIterators.empty()) objectIterators.pop();
    }
}

void DataNodeInputStream::readBeginArray() {
    if (auto* node = getNextNode(); node && node->isArray()) {
        nodeStack.push(node);
        arrayIndices.push(0);
    }
}

void DataNodeInputStream::readEndArray() {
    if (nodeStack.size() > 1) {
        nodeStack.pop();
        if (!arrayIndices.empty()) arrayIndices.pop();
    }
}
