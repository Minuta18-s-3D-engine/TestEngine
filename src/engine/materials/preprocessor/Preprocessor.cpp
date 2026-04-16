#include "Preprocessor.hpp"

Preprocessor::Preprocessor(
    FilesystemAbstraction& _filesystem
) : filesystem(_filesystem), 
    cache(_filesystem, "fs://.cache/materialCode") {}

std::shared_ptr<PreprocessorParser> Preprocessor::createParser(
    const std::string& fileContents
) {
    auto parser = std::make_shared<PreprocessorParser>(fileContents);

    using ArgType = PreprocessorParser::ArgType;
    parser->addDirectiveValidator(
        "import", { ArgType::String, ArgType::String }
    );

    return parser;
}

void Preprocessor::preprocess(const VirtualPath& filePath) {
    std::string fileContents = filesystem.readFile(filePath);
    
    auto parser = createParser(fileContents);
    auto result = parser->parse();
}
