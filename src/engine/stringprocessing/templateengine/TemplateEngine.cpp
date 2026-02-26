#include "TemplateEngine.hpp"

TemplateEngine::TemplateEngine(
    VirtualPath _templateDirectory, 
    const Parser& _parser
) : templateDirectory(_templateDirectory), parser(_parser) {}

std::string TemplateEngine::readFile(const VirtualPath& path) const {
    std::ifstream file(
        path.resolve(), 
        std::ios::binary | std::ios::ate
    );
    if (!file.is_open()) {
        throw exc::file_not_found(
            "Template " + path.getVirtual() + " not found"
        );
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::string fileContents;
    fileContents.resize(static_cast<size_t>(size));

    if (!file.read(&fileContents[0], size)) {
        throw exc::file_corrupted(
            "Failed to read template " + path.getVirtual()
        );
    }

    return fileContents;
}


VirtualPath TemplateEngine::getPathByName(
    const std::string& templateName
) const {
    return VirtualPath(templateDirectory.getVirtual() + "/" + templateName);
}

std::string TemplateEngine::render(
    const std::string& templateName, 
    const TemplateArguments& arguments
) const {
    return render(getPathByName(templateName), arguments);    
}

std::string TemplateEngine::render(
    const VirtualPath& templatePath, 
    const TemplateArguments& arguments
) const {
    std::string fileContents = readFile(templatePath);

    return parser.renderFromString(fileContents, arguments);
}

ErrorLog TemplateEngine::validate(const std::string& templateName) const {
    return validate(getPathByName(templateName));
}

ErrorLog TemplateEngine::validate(const VirtualPath& templatePath) const {
    std::string fileContents = readFile(templatePath);

    return parser.validateFromString(fileContents);
}
