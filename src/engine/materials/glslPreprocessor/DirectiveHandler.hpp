#ifndef ENGINE_MATERIALS_GLSLPREPROCESSOR_DIRECTIVEHANDLER_H_
#define ENGINE_MATERIALS_GLSLPREPROCESSOR_DIRECTIVEHANDLER_H_

#include "PreprocessorParserContext.hpp"

class DirectiveHandler {
    virtual void handleDirective(PreprocessorParserContext& context) = 0;
};

#endif // ENGINE_MATERIALS_GLSLPREPROCESSOR_DIRECTIVEHANDLER_H_
