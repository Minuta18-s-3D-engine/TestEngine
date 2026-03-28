#ifndef ENGINE_UTILS_ENUMMAPPER_H_
#define ENGINE_UTILS_ENUMMAPPER_H_

#include <unordered_map>
#include <string>
#include <initializer_list>
#include <exception>
#include <stdexcept>

template <typename T>
class EnumMapper {
    std::unordered_map<std::string, T> stringToEnum;
    std::unordered_map<T, std::string> enumToString;
public:
    EnumMapper(
        std::initializer_list<std::pair<T, std::string>> mapping
    ) {
        for (const auto& [value, name] : mapping) {
            enumToString[value] = name;
            stringToEnum[name] = value;
        }
    }

    T toEnum(const std::string& _string) const { 
        if (!stringToEnum.contains(_string)) {
            throw std::out_of_range("unknown enum value."); 
        }
        return stringToEnum.at(_string); 
    }

    std::string toString(T _t) const { 
        if (!enumToString.contains(_t)) {
            throw std::out_of_range("unknown enum value.");
        }
        return enumToString.at(_t); 
    }
};


#endif // ENGINE_UTILS_ENUMMAPPER_H_
