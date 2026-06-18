#ifndef ENGINE_DEBUG_LOGGING_LOGGERFILEPOOL_HPP_
#define ENGINE_DEBUG_LOGGING_LOGGERFILEPOOL_HPP_

#include <unordered_map>
#include <string>
#include <memory>

#include "../../project/File.hpp"
#include "../../project/VirtualPath.hpp"

class LoggerFilePool {
    std::unordered_map<std::string, std::shared_ptr<File>> pool;
public:
    std::shared_ptr<File> openFile(const VirtualPath& path, File::IOMode mode);
};

#endif // ENGINE_DEBUG_LOGGING_LOGGERFILEPOOL_HPP_