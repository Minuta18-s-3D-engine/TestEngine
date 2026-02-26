#ifndef ENGINE_STRINGPROCESSING_TEMPLATEENGINE_TEMPLATEENGINE_HPP
#define ENGINE_STRINGPROCESSING_TEMPLATEENGINE_TEMPLATEENGINE_HPP

#include <string>
#include <unordered_map>
#include <fstream>

#include "../../project/VirtualPath.hpp"
#include "TemplateArguments.hpp"
#include "../utils/ParseExceptions.hpp"
#include "../utils/Parser.hpp"
#include "../utils/exc/GeneralExceptions.hpp"

class TemplateEngine {
private:
    VirtualPath templateDirectory;
    const Parser& parser;

    std::string readFile(const VirtualPath& path) const;
public:
    TemplateEngine(VirtualPath _templateDirectory, const Parser& _parser);

    VirtualPath getPathByName(const std::string& templateName) const;

    std::string render(
        const std::string& templateName, 
        const TemplateArguments& arguments = TemplateArguments{}
    ) const;

    std::string render(
        const char* templateName, 
        const TemplateArguments& arguments = TemplateArguments{}
    ) const;

    std::string render(
        const VirtualPath& templatePath, 
        const TemplateArguments& arguments = TemplateArguments{}
    ) const;

    ErrorLog validate(const std::string& templateName) const;
    ErrorLog validate(const char* templateName) const;
    ErrorLog validate(const VirtualPath& templatePath) const;
};

#endif // ENGINE_STRINGPROCESSING_TEMPLATEENGINE_TEMPLATEENGINE_HPP
