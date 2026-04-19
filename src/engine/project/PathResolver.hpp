#ifndef ENGINE_PROJECT_PATHRESOLVER_HPP_
#define ENGINE_PROJECT_PATHRESOLVER_HPP_

#include <string>

class PathResolver {
public:
    virtual ~PathResolver() = default;

    virtual void addPrefix(std::string name, std::filesystem::path path) = 0;

    virtual std::string resolve(const std::string& virtualPath) const = 0; 
    virtual std::string resolveFrom(
        const std::string& virtualPath,
        const std::string& folder 
    ) const = 0; 
};

#endif // ENGINE_PROJECT_PATHRESOLVER_HPP_
