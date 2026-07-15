#include "DataNode.hpp"
#include <new>
#include <stdexcept>
#include <utility>

namespace StructuredData {

DataNode& Object::operator[](const key_t& key) {
    for (auto& entry : entries) {
        if (entry.first == key) {
            return entry.second;
        }
    }
    entries.emplace_back(key, DataNode());
    return entries.back().second;
}

const DataNode* Object::find(const key_t& key) const noexcept {
    for (const auto& entry : entries) {
        if (entry.first == key) {
            return &entry.second;
        }
    }
    return nullptr;
}

DataNode* Object::find(const key_t& key) noexcept {
    for (auto& entry : entries) {
        if (entry.first == key) {
            return &entry.second;
        }
    }
    return nullptr;
}

bool Object::erase(const key_t& key) noexcept {
    for (auto it = entries.begin(); it != entries.end(); ++it) {
        if (it->first == key) {
            entries.erase(it);
            return true;
        }
    }
    return false;
}

DataNode::DataNode(const std::string& value) : type(Type::String) {
    new (&value_string) std::string(value);
}

DataNode::DataNode(std::string&& value) noexcept : type(Type::String) {
    new (&value_string) std::string(std::move(value));
}

DataNode::DataNode(const char* value) : type(Type::String) {
    new (&value_string) std::string(value);
}

DataNode::DataNode(const Array& value) : type(Type::Array) {
    new (&value_array) Array(value);
}

DataNode::DataNode(Array&& value) noexcept : type(Type::Array) {
    new (&value_array) Array(std::move(value));
}

DataNode::DataNode(const Object& value) : type(Type::Object) {
    new (&value_object) Object(value);
}

DataNode::DataNode(Object&& value) noexcept : type(Type::Object) {
    new (&value_object) Object(std::move(value));
}

DataNode::~DataNode() noexcept {
    switch (type) {
        case Type::String:
            value_string.~basic_string();
            break;
        case Type::Array:
            value_array.~vector();
            break;
        case Type::Object:
            value_object.~Object();
            break;
        default:
            break;
    }
}

DataNode::DataNode(const DataNode& other) : type(other.type) {
    switch (type) {
        case Type::Boolean: value_boolean = other.value_boolean; break;
        case Type::Integer: value_integer = other.value_integer; break;
        case Type::Number:  value_number = other.value_number; break;
        case Type::String:  
            new (&value_string) std::string(other.value_string); break;
        case Type::Array:   
            new (&value_array) Array(other.value_array); break;
        case Type::Object:  
            new (&value_object) Object(other.value_object); break;
        default: break;
    }
}

DataNode::DataNode(DataNode&& other) noexcept : type(other.type) {
    switch (type) {
        case Type::Boolean: value_boolean = other.value_boolean; break;
        case Type::Integer: value_integer = other.value_integer; break;
        case Type::Number:  value_number = other.value_number; break;
        case Type::String:  
            new (&value_string) std::string(std::move(other.value_string)); 
            break;
        case Type::Array:   
            new (&value_array) Array(std::move(other.value_array)); break;
        case Type::Object:  
            new (&value_object) Object(std::move(other.value_object)); break;
        default: break;
    }
    other.type = Type::Null;
}

DataNode& DataNode::operator=(const DataNode& other) {
    if (this != &other) {
        this->~DataNode();
        type = other.type;
        switch (type) {
            case Type::Boolean: value_boolean = other.value_boolean; break;
            case Type::Integer: value_integer = other.value_integer; break;
            case Type::Number:  value_number = other.value_number; break;
            case Type::String:  
                new (&value_string) std::string(other.value_string); break;
            case Type::Array:   
                new (&value_array) Array(other.value_array); break;
            case Type::Object:  
                new (&value_object) Object(other.value_object); break;
            default: break;
        }
    }
    return *this;
}

DataNode& DataNode::operator=(DataNode&& other) noexcept {
    if (this != &other) {
        this->~DataNode();
        type = other.type;
        switch (type) {
            case Type::Boolean: value_boolean = other.value_boolean; break;
            case Type::Integer: value_integer = other.value_integer; break;
            case Type::Number:  value_number = other.value_number; break;
            case Type::String:  
                new (&value_string) std::string(std::move(other.value_string));
                break;
            case Type::Array:   
                new (&value_array) Array(std::move(other.value_array)); 
                break;
            case Type::Object:  
                new (&value_object) Object(std::move(other.value_object)); 
                break;
            default: break;
        }
        other.type = Type::Null;
    }
    return *this;
}

bool DataNode::asBoolean() const {
    if (type != Type::Boolean) 
        throw std::runtime_error("DataNode is not a boolean");
    return value_boolean;
}

int64_t DataNode::asInteger() const {
    if (type != Type::Integer) 
        throw std::runtime_error("DataNode is not an integer");
    return value_integer;
}

double DataNode::asNumber() const {
    if (type == Type::Number) return value_number;
    if (type == Type::Integer) return static_cast<double>(value_integer);
    throw std::runtime_error("DataNode is not a number");
}

const std::string& DataNode::asString() const {
    if (type != Type::String)
        throw std::runtime_error("DataNode is not a string");
    return value_string;
}

const Array& DataNode::asArray() const {
    if (type != Type::Array) 
        throw std::runtime_error("DataNode is not an array");
    return value_array;
}

const Object& DataNode::asObject() const {
    if (type != Type::Object) 
        throw std::runtime_error("DataNode is not an object");
    return value_object;
}

Array& DataNode::asArray() {
    if (type != Type::Array) 
        throw std::runtime_error("DataNode is not an array");
    return value_array;
}

Object& DataNode::asObject() {
    if (type != Type::Object) 
        throw std::runtime_error("DataNode is not an object");
    return value_object;
}

bool DataNode::asBoolean(bool defaultValue) const {
    return (type == Type::Boolean) ? value_boolean : defaultValue;
}

int64_t DataNode::asInteger(int64_t defaultValue) const {
    return (type == Type::Integer) ? value_integer : defaultValue;
}

double DataNode::asNumber(double defaultValue) const {
    if (type == Type::Number) return value_number;
    if (type == Type::Integer) return static_cast<double>(value_integer);
    return defaultValue;
}

const std::string& DataNode::asString(const std::string& defaultValue) const {
    return (type == Type::String) ? value_string : defaultValue;
}

const Array& DataNode::asArray(const Array& defaultValue) const {
    return (type == Type::Array) ? value_array : defaultValue;
}

const Object& DataNode::asObject(const Object& defaultValue) const {
    return (type == Type::Object) ? value_object : defaultValue;
}

DataNode& DataNode::operator[](const key_t& key) {
    if (type == Type::Null) {
        new (&value_object) Object();
        type = Type::Object;
    } else if (type != Type::Object) {
        throw std::runtime_error(
            "Attempted to index non-object DataNode with string key: " + key
        );
    }
    return value_object[key];
}

const DataNode& DataNode::operator[](const key_t& key) const {
    if (type != Type::Object) {
        throw std::runtime_error(
            "Attempted to index non-object DataNode with string key: " + key
        );
    }
    const DataNode* found = value_object.find(key);
    if (!found) {
        throw std::runtime_error("Key not found in DataNode: " + key);
    }
    return *found;
}

DataNode& DataNode::operator[](size_t index) {
    if (type == Type::Null) {
        new (&value_array) Array();
        type = Type::Array;
    } else if (type != Type::Array) {
        throw std::runtime_error(
            "Attempted to index non-array DataNode with integer index"
        );
    }
    if (index >= value_array.size()) {
        value_array.resize(index + 1);
    }
    return value_array[index];
}

const DataNode& DataNode::operator[](size_t index) const {
    if (type != Type::Array) {
        throw std::runtime_error(
            "Attempted to index non-array DataNode with integer index"
        );
    }
    if (index >= value_array.size()) {
        throw std::runtime_error("DataNode array index out of bounds");
    }
    return value_array[index];
}

} // namespace StructuredData
