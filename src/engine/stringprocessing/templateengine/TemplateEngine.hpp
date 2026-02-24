#ifndef ENGINE_STRINGPROCESSING_TEMPLATEENGINE_TEMPLATEENGINE_HPP
#define ENGINE_STRINGPROCESSING_TEMPLATEENGINE_TEMPLATEENGINE_HPP

#include <string>
#include <unordered_map>

#include "../../project/VirtualPath.hpp"
#include "TemplateArguments.hpp"
#include "../utils/ParseExceptions.hpp"
#include "../utils/Parser.hpp"

class TemplateEngine {
private:
    VirtualPath templateDirectory;
    Parser& parser;
public:
    TemplateEngine(VirtualPath templateDirectory, const Parser& parser);

    std::string render(
        const std::string& templateName, 
        const TemplateArguments& arguments = TemplateArguments{}
    ) const;

    std::string render(
        const VirtualPath& templatePath, 
        const TemplateArguments& arguments = TemplateArguments{}
    ) const;

    ErrorLog validate(const std::string& templateName) const;
    ErrorLog validate(const VirtualPath& templatePath) const;
};

#endif // ENGINE_STRINGPROCESSING_TEMPLATEENGINE_TEMPLATEENGINE_HPP
