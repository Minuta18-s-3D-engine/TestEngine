#ifndef ENGINE_UTILS_ENUMMAPPER_H_
#define ENGINE_UTILS_ENUMMAPPER_H_

#include <unordered_map>
#include <string>
#include <initializer_list>

template <typename T>
class EnumMapper {
    std::unordered_map<std::string, T> stringToEnum;
    std::unordered_map<T, std::string> enumToString;
public:
    EnumMapper(
        std::initializer_list<std::pair<T, std::string>> mapping
    ) {
        for (const auto& [value, name] : mapping) {
            enumToString[name] = value;
            stringToEnum[value] = name;
        }
    }

    T toEnum(const std::string& _string) { return stringToEnum[_string]; }
    std::string toString(T _t) { return enumToString[_t]; }
};

#endif // ENGINE_UTILS_ENUMMAPPER_H_
