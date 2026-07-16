#include "JsonSerializer.hpp"

StructuredData::DataNode JsonSerializer::fromFile(const VirtualPath& path) {
    File jsonFile(path);

    std::string content = jsonFile.readAll();
    
    if (content.empty()) {
        throw std::runtime_error(
            "Failed to read file or file is empty: " + path.resolve()
        );
    }
    
    return fromString(content);
}

void JsonSerializer::toFile(
    const VirtualPath& path, const StructuredData::DataNode& node
) {
    std::string content = toString(node);
    
    File jsonFile(path, File::IOMode::WRITE);
    jsonFile.write(content);
}

StructuredData::DataNode JsonSerializer::fromString(const std::string& str) {
    nlohmann::json j = nlohmann::json::parse(str);
    return fromJson(j);
}

std::string JsonSerializer::toString(
    const StructuredData::DataNode& node
) {
    nlohmann::json j = toJson(node);
    
    int indent = currentOptions.minimize ? -1 : currentOptions.indent;
    
    return j.dump(indent); 
}

StructuredData::DataNode JsonSerializer::fromJson(const nlohmann::json& j) {
    switch (j.type()) {
        case nlohmann::json::value_t::null:
            return StructuredData::DataNode();
        case nlohmann::json::value_t::boolean:
            return StructuredData::DataNode(j.get<bool>());
        case nlohmann::json::value_t::number_integer:
        case nlohmann::json::value_t::number_unsigned:
            return StructuredData::DataNode(j.get<int64_t>());
        case nlohmann::json::value_t::number_float:
            return StructuredData::DataNode(j.get<double>());
        case nlohmann::json::value_t::string:
            return StructuredData::DataNode(j.get<std::string>());
        case nlohmann::json::value_t::array: {
            StructuredData::Array arr;
            arr.reserve(j.size());
            for (const auto& item : j) {
                arr.push_back(fromJson(item));
            }
            return StructuredData::DataNode(std::move(arr));
        }
        case nlohmann::json::value_t::object: {
            StructuredData::Object obj;
            for (const auto& [key, value] : j.items()) {
                obj[key] = fromJson(value); 
            }
            return StructuredData::DataNode(std::move(obj));
        }
        default:
            throw std::runtime_error(
                "Unsupported nlohmann::json type encountered"
        );
    }
}

nlohmann::json JsonSerializer::toJson(const StructuredData::DataNode& node) {
    switch (node.getType()) {
        case StructuredData::Type::Null:
            return nullptr;
        case StructuredData::Type::Boolean:
            return node.asBoolean();
        case StructuredData::Type::Integer:
            return node.asInteger();
        case StructuredData::Type::Number:
            return node.asNumber();
        case StructuredData::Type::String:
            return node.asString();
        case StructuredData::Type::Array: {
            nlohmann::json j = nlohmann::json::array();
            for (const auto& item : node.asArray()) {
                j.push_back(toJson(item));
            }
            return j;
        }
        case StructuredData::Type::Object: {
            nlohmann::json j = nlohmann::json::object();
            for (const auto& entry : node.asObject()) {
                j[entry.first] = toJson(entry.second);
            }
            return j;
        }
        default:
            throw std::runtime_error("Unsupported DataNode Type");
    }
}
