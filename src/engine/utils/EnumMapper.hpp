#ifndef ENGINE_UTILS_ENUMMAPPER_H_
#define ENGINE_UTILS_ENUMMAPPER_H_

#include <unordered_map>
#include <string>
#include <initializer_list>
#include <exception>
#include <stdexcept>

template <typename T>
class EnumMapper {
public:
    struct Mapping {
        std::string_view name;
        T value;
    };
private:
    std::vector<Mapping> mappings;
public:
    EnumMapper(
        std::initializer_list<std::pair<T, std::string>> mappings_
    ) {
        mappings.reserve(mappings_.size());
        for (int i = 0; i < mappings_.size(); ++i) {
            mappings.push_back({ mappings_[i].second, mappings_[i].first });
        }
    }

    [[nodiscard]] T toEnum(const std::string& name) const {
        for (const auto& m : mappings) {
            if (m.name == name) return m.value;
        }
        throw std::out_of_range("Unknown enum string name.");
    }

    [[nodiscard]] std::string toString(T value) const {
        for (const auto& m : mappings) {
            if (m.value == value) return m.name;
        }
        throw std::out_of_range("Unknown enum value.");
    }

    [[nodiscard]] std::optional<T> tryToEnum(std::string_view name) const {
        for (const auto& m : mappings) {
            if (m.name == name) return m.value;
        }
        return std::nullopt;
    }
};


#endif // ENGINE_UTILS_ENUMMAPPER_H_
