#include "LoggerFilePool.hpp"

std::shared_ptr<File> LoggerFilePool::openFile(
    const VirtualPath& path, 
    File::IOMode mode
) {
    std::string key = path.resolve();

    auto it = pool.find(key);
    if (it != pool.end()) {
        return it->second;
    }

    auto newFile = std::make_shared<File>(path, mode);
    pool[key] = newFile;

    return newFile;
}