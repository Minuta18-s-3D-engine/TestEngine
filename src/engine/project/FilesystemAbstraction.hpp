#ifndef ENGINE_PROJECT_FILESYSTEMABSTRACTION_HPP_
#define ENGINE_PROJECT_FILESYSTEMABSTRACTION_HPP_

#include <string>
#include <filesystem>

#include "VirtualPath.hpp"
#include "PathResolver.hpp"

class FilesystemAbstraction {
public:
    virtual ~FilesystemAbstraction() = default;

    virtual bool exists(const VirtualPath& path) = 0;

    virtual std::string readFile(const VirtualPath& path) = 0;
    virtual void writeFile(
        const VirtualPath& path, 
        const std::string& data, 
        bool overwrite = true
    ) = 0;
    virtual bool fileExists(const VirtualPath& path) = 0;
    virtual bool isFile(const VirtualPath& path) = 0;
    virtual std::filesystem::file_time_type getLastEditedTime(
        const VirtualPath& path
    ) = 0;

    virtual bool folderExists(const VirtualPath& path) = 0;
    virtual void createFolder(const VirtualPath& path) = 0;
    virtual void removeFolder(
        const VirtualPath& path, 
        bool recursively = false
    ) = 0;

    virtual PathResolver& getResolver() = 0;
};

#endif // ENGINE_PROJECT_FILESYSTEMABSTRACTION_HPP_
