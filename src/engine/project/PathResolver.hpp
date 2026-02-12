#ifndef ENGINE_PROJECT_PATHRESOLVER_HPP_
#define ENGINE_PROJECT_PATHRESOLVER_HPP_

#include <filesystem>
#include <string>

class PathResolver {
    const std::string prefix = "fs://";
    const std::string corePrefix = "core://";
    
    std::filesystem::path root;
    std::filesystem::path coreRoot;
public:
    PathResolver(
        const std::filesystem::path& _root,
        const std::filesystem::path& _coreRoot
    );

    std::filesystem::path resolve(const std::string& virtualPath) const; 
};

#endif // ENGINE_PROJECT_PATHRESOLVER_HPP_
