#ifndef ENGINE_UTILS_ENUMMAPPER_H_
#define ENGINE_UTILS_ENUMMAPPER_H_

#include <string>
#include <initializer_list>
#include <optional>
#include <vector>
#include <stdexcept>

template <typename T>
class EnumMapper {
public:
    struct Mapping {
        std::string name;
        T value;
    };
private:
    std::vector<Mapping> mappings;
public:
    EnumMapper(
        std::initializer_list<std::pair<T, std::string>> mappings_
    ) {
        mappings.reserve(mappings_.size());
        for (auto [value, name] : mappings_) {
            mappings.push_back({
                .name = name,
                .value = value
            });
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
            if (m.value == value) return std::string(m.name);
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
