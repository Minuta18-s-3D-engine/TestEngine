#ifndef ENGINE_STRINGPROCESSING_TEMPLATEENGINE_TEMPLATEARGUMENTS_HPP
#define ENGINE_STRINGPROCESSING_TEMPLATEENGINE_TEMPLATEARGUMENTS_HPP

#include <string>
#include <unordered_map>

#include "../utils/Parser.hpp"

class TemplateArguments : public ParserArguments {
public:
    using ArgumentMap = std::unordered_map<std::string, std::string>;
private:
    ArgumentMap arguments;
public:
    TemplateArguments() = default;
    TemplateArguments(const ArgumentMap& args) : arguments(args) {}
    TemplateArguments(const TemplateArguments& other);
    TemplateArguments& operator=(const TemplateArguments& other);
    TemplateArguments(TemplateArguments&& other) noexcept;
    TemplateArguments& operator=(TemplateArguments&& other) noexcept;

    TemplateArguments& set(
        const std::string& key, const std::string& value
    ) override;
    std::string get(
        const std::string& key, const std::string& defaultValue
    ) const override;

    ArgumentMap getArguments() const;
};

#endif // ENGINE_STRINGPROCESSING_TEMPLATEENGINE_TEMPLATEARGUMENTS_HPP