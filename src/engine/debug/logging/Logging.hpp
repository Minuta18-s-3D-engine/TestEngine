#ifndef ENGINE_DEBUG_LOGGING_LOGGING_HPP_
#define ENGINE_DEBUG_LOGGING_LOGGING_HPP_

#include "Logger.hpp"

namespace Logging {

void setDefaultConfig();

Logger createLogger(std::string loggerName);

};

#endif // ENGINE_DEBUG_LOGGING_LOGGING_HPP_
