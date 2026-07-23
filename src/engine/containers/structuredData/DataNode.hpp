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
    [[nodiscard]] const DataNode* find(const key_t& key) const noexcept;
    [[nodiscard]] DataNode* find(const key_t& key) noexcept;
    bool erase(const key_t& key) noexcept;

    [[nodiscard]] std::vector<Entry>::iterator begin() noexcept { return entries.begin(); }
    [[nodiscard]] std::vector<Entry>::iterator end() noexcept { return entries.end(); }
    [[nodiscard]] std::vector<Entry>::const_iterator begin() const noexcept {
        return entries.begin(); 
    } 
    [[nodiscard]] std::vector<Entry>::const_iterator end() const noexcept {
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
    DataNode() : type(Type::Null), value_boolean(false) {}
    explicit DataNode(bool value) : type(Type::Boolean), value_boolean(value) {}
    explicit DataNode(int64_t value) : type(Type::Integer), value_integer(value) {}
    explicit DataNode(double value) : type(Type::Number), value_number(value) {}

    explicit DataNode(const std::string& value);
    explicit DataNode(std::string&& value) noexcept;
    explicit DataNode(const char* value);
    explicit DataNode(const Array& value);
    explicit DataNode(Array&& value) noexcept;
    explicit DataNode(const Object& value);
    explicit DataNode(Object&& value) noexcept;
    
    ~DataNode() noexcept;
    DataNode(const DataNode& other);
    DataNode(DataNode&& other) noexcept;
    DataNode& operator=(const DataNode& other);
    DataNode& operator=(DataNode&& other) noexcept;

    [[nodiscard]] Type getType()   const noexcept { return type; }
    [[nodiscard]] bool isNull()    const noexcept { return type == Type::Null; }
    [[nodiscard]] bool isBoolean() const noexcept { return type == Type::Boolean; }
    [[nodiscard]] bool isInteger() const noexcept { return type == Type::Integer; }
    [[nodiscard]] bool isNumber()  const noexcept { return type == Type::Integer || type == Type::Number; }
    [[nodiscard]] bool isString()  const noexcept { return type == Type::String; }
    [[nodiscard]] bool isArray()   const noexcept { return type == Type::Array; }
    [[nodiscard]] bool isObject()  const noexcept { return type == Type::Object; }

    [[nodiscard]] bool asBoolean() const;
    [[nodiscard]] int64_t asInteger() const;
    [[nodiscard]] double asNumber() const;
    [[nodiscard]] const std::string& asString() const;
    [[nodiscard]] const Array& asArray() const;
    [[nodiscard]] const Object& asObject() const;

    [[nodiscard]] bool asBoolean(bool defaultValue) const;
    [[nodiscard]] int64_t asInteger(int64_t defaultValue) const;
    [[nodiscard]] double asNumber(double defaultValue) const;
    [[nodiscard]] const std::string& asString(const std::string& defaultValue) const;
    [[nodiscard]] const Array& asArray(const Array& defaultValue) const;
    [[nodiscard]] const Object& asObject(const Object& defaultValue) const;

    Array& asArray();
    Object& asObject();

    DataNode& operator[](const key_t& key);
    const DataNode& operator[](const key_t& key) const;
    DataNode& operator[](size_t index);
    const DataNode& operator[](size_t index) const;
};

}; // StructuredData

#endif // ENGINE_CONTAINERS_STRUCTUREDDATA_DATANODE_HPP_
