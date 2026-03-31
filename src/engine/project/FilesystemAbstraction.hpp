#ifndef ENGINE_PROJECT_FILESYSTEMABSTRACTION_HPP_
#define ENGINE_PROJECT_FILESYSTEMABSTRACTION_HPP_

#include <string>

#include "VirtualPath.hpp"

class FilesystemAbstraction {
public:
    virtual ~FilesystemAbstraction() = default;

    virtual bool exists(const VirtualPath& path);

    virtual std::string readFile(const VirtualPath& path);
    virtual void writeFile(
        const VirtualPath& path, 
        const std::string& data, 
        bool overwrite = true
    );
    virtual bool fileExists(const VirtualPath& path);

    virtual bool dirExists(const VirtualPath& path);
    virtual void createDir(const VirtualPath& path);
    virtual void removeDir(const VirtualPath& path, bool recursively = false);
};

#endif // ENGINE_PROJECT_FILESYSTEMABSTRACTION_HPP_
