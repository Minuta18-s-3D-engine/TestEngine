#ifndef ENGINE_PROJECT_VIRTUAL_PATH_HPP_
#define ENGINE_PROJECT_VIRTUAL_PATH_HPP_

#include <string>
#include <filesystem>
#include <memory>

#include <GeneralPathResolver.hpp>

class VirtualPath {
    static std::shared_ptr<PathResolver> resolver;

    std::string path;
public:
    VirtualPath();
    VirtualPath(const std::string& _path);
    VirtualPath(const char* _path);
    VirtualPath(const std::filesystem::path& _path);

    std::string resolve() const;
    std::string resolveFrom(const VirtualPath& folder) const;
    const std::string& getVirtual() const { return path; }

    static void setResolverFunc(std::shared_ptr<PathResolver> _resolver) { 
        resolver = _resolver; 
    }
};

#endif // ENGINE_PROJECT_VIRTUAL_PATH_HPP_
