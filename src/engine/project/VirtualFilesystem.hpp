#ifndef ENGINE_PROJECT_VIRTUALFILESYSTEM_HPP_
#define ENGINE_PROJECT_VIRTUALFILESYSTEM_HPP_

#include <fstream>
#include <memory>

#include "FilesystemAbstraction.hpp"
#include "../utils/exc/GeneralExceptions.hpp"
#include "PathResolver.hpp"

class VirtualFilesystem : public FilesystemAbstraction {
    std::unique_ptr<PathResolver> pathResolver;
public:
    VirtualFilesystem(std::unique_ptr<PathResolver> _pathResolver);
    ~VirtualFilesystem() override;

    bool exists(const VirtualPath& path) override;

    std::string readFile(const VirtualPath& path) override;
    void writeFile(
        const VirtualPath& path, 
        const std::string& data, 
        bool overwrite = true
    ) override;
    bool fileExists(const VirtualPath& path) override;
    bool isFile(const VirtualPath& path) override;
    std::filesystem::file_time_type getLastEditedTime(
        const VirtualPath& path
    ) override;

    bool dirExists(const VirtualPath& path) override;
    void createDir(const VirtualPath& path) override;
    void removeDir(const VirtualPath& path, bool recursively = false) override;

    PathResolver& getResolver() override;
};

#endif // ENGINE_PROJECT_VIRTUALFILESYSTEM_HPP_
