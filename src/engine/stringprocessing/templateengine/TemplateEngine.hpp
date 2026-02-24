#ifndef ENGINE_STRINGPROCESSING_TEMPLATEENGINE_TEMPLATEENGINE_HPP
#define ENGINE_STRINGPROCESSING_TEMPLATEENGINE_TEMPLATEENGINE_HPP

#include <string>
#include <unordered_map>

#include "../project/VirtualPath.hpp"
#include "TemplateArguments.hpp"

class TemplateEngine {
private:
    void renderFromString(
        const std::string& fileContents,
        const TemplateArguments& arguments
    ) const;
public:
    TemplateEngine(VirtualPath templateDirectory);

    std::string render(
        const std::string& templateName, 
        const TemplateArguments& arguments = TemplateArguments{}
    ) const;

    std::string render(
        const VirtualPath& templatePath, 
        const TemplateArguments& arguments = TemplateArguments{}
    ) const;
};

#endif // ENGINE_STRINGPROCESSING_TEMPLATEENGINE_TEMPLATEENGINE_HPP
