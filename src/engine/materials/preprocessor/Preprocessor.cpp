#include "Preprocessor.hpp"

Preprocessor::Preprocessor(
    FilesystemAbstraction& _filesystem
) : filesystem(_filesystem), 
    cache(_filesystem, "fs://.cache/materialCode") {}

void Preprocessor::preprocess(const VirtualPath& filePath) {
    std::string fileContents = filesystem.readFile(filePath);
    PreprocessorParser parser(fileContents);

    using ArgType = PreprocessorParser::ArgType;
    parser.addDirectiveValidator("import", { ArgType::String });
}
