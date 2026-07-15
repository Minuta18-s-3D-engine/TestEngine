#ifndef ENGINE_CONTAINERS_STRUCTUREDDATA_DATANODE_HPP_
#define ENGINE_CONTAINERS_STRUCTUREDDATA_DATANODE_HPP_

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include "engine/utils/EnumMapper.hpp"

namespace StructuredData {

class DataNode;

using key_t = std::string;

using Array = std::vector<DataNode>;

class Object {
public:
    using Entry = std::pair<key_t, DataNode>;

    DataNode& operator[](const key_t& key);
    const DataNode* find(const key_t& key) const noexcept;
    DataNode* find(const key_t& key) noexcept;
    bool erase(const key_t& key) noexcept;

    std::vector<Entry>::iterator begin() noexcept { return entries.begin(); } 
    std::vector<Entry>::iterator end() noexcept { return entries.end(); } 
    std::vector<Entry>::const_iterator begin() const noexcept { 
        return entries.begin(); 
    } 
    std::vector<Entry>::const_iterator end() const noexcept { 
        return entries.end(); 
    } 
private:
    std::vector<Entry> entries;
};

enum class Type : uint8_t {
    Null = 0,
    Boolean,
    Integer,
    Number,
    String,
    Array,
    Object
};

const EnumMapper<Type> typeMapper = {
    { Type::Null, "null" },
    { Type::Boolean, "boolean" },
    { Type::Integer, "integer" },
    { Type::Number, "number" },
    { Type::String, "string" },
    { Type::Array, "array" },
    { Type::Object, "object" }
}; 

class DataNode {
    Type type = Type::Null;

    union {
        bool value_boolean;
        int64_t value_integer;
        double value_number;
        std::string value_string;
        Array value_array;
        Object value_object;
    };
public:
    DataNode() : type(Type::Null) {}
    DataNode(bool value) : value_boolean(value), type(Type::Boolean) {}
    DataNode(int64_t value) : value_integer(value), type(Type::Integer) {}
    DataNode(double value) : value_number(value), type(Type::Number) {}

    DataNode(const std::string& value);
    DataNode(std::string&& value) noexcept;
    DataNode(const char* value);
    DataNode(const Array& value);
    DataNode(Array&& value) noexcept;
    DataNode(const Object& value);
    DataNode(Object&& value) noexcept;
    
    ~DataNode() noexcept;
    DataNode(const DataNode& other);
    DataNode(DataNode&& other) noexcept;
    DataNode& operator=(const DataNode& other);
    DataNode& operator=(DataNode&& other) noexcept;

    Type getType()   const noexcept { return type; } 
    bool isNull()    const noexcept { return type == Type::Null; }
    bool isBoolean() const noexcept { return type == Type::Boolean; }
    bool isInteger() const noexcept { return type == Type::Integer; }
    bool isNumber()  const noexcept { return type == Type::Integer || type == Type::Number; }
    bool isString()  const noexcept { return type == Type::String; }
    bool isArray()   const noexcept { return type == Type::Array; }
    bool isObject()  const noexcept { return type == Type::Object; }

    bool asBoolean() const;
    int64_t asInteger() const;
    double asNumber() const;
    const std::string& asString() const;
    const Array& asArray() const;
    const Object& asObject() const;

    bool asBoolean(bool default) const;
    int64_t asInteger(int64_t default) const;
    double asNumber(double default) const;
    const std::string& asString(const std::string& default) const;
    const Array& asArray(const Array& default) const;
    const Object& asObject(const Object& default) const;

    Array& asArray();
    Object& asObject();

    DataNode& operator[](const key_t& key);
    const DataNode& operator[](const key_t& key) const;
    DataNode& operator[](size_t index);
    const DataNode& operator[](size_t index) const;
};

}; // StructuredData

#endif // ENGINE_CONTAINERS_STRUCTUREDDATA_DATANODE_HPP_
