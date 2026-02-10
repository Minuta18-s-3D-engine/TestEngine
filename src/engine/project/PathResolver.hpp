#ifndef ENGINE_PROJECT_PATHRESOLVER_HPP_
#define ENGINE_PROJECT_PATHRESOLVER_HPP_

#include <filesystem>
#include <string>

class PathResolver {
    const std::string prefix = "fs://";
    
    std::filesystem::path root;
public:
    PathResolver(const std::filesystem::path& _root);

    std::filesystem::path resolve(const std::string& virtualPath) const;
};

#endif // ENGINE_PROJECT_PATHRESOLVER_HPP_
