#include "TemplateArguments.hpp"

TemplateArguments::TemplateArguments(
    const TemplateArguments& other
) : arguments(other.arguments) {}

TemplateArguments& TemplateArguments::operator=(
    const TemplateArguments& other
) {
    if (this != &other) {
        arguments = other.arguments;
    }
    return *this;
}

TemplateArguments::TemplateArguments(
    TemplateArguments&& other
) noexcept : arguments(std::move(other.arguments)) {}

TemplateArguments& TemplateArguments::operator=(
    TemplateArguments&& other
) noexcept {
    if (this != &other) {
        arguments = std::move(other.arguments);
    }
    return *this;
}

void TemplateArguments::set(const std::string& key, const std::string& value) {
    arguments[key] = value;
}

std::string TemplateArguments::get(
    const std::string& key, const std::string& defaultValue
) const {
    auto it = arguments.find(key);
    if (it != arguments.end()) {
        return it->second;
    }
    return defaultValue;
}

TemplateArguments::ArgumentMap TemplateArguments::getArguments() const {
    return arguments;
}
