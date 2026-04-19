#ifndef ENGINE_PROJECT_GENERALPATHRESOLVER_HPP_
#define ENGINE_PROJECT_GENERALPATHRESOLVER_HPP_

#include <filesystem>
#include <string>
#include <unordered_map>

#include "PathResolver.hpp"

class GeneralPathResolver : public PathResolver {
    std::unordered_map<std::string, std::filesystem::path> prefixes;
public:
    GeneralPathResolver() = default;
    ~GeneralPathResolver() override = default;

    void addPrefix(std::string name, std::filesystem::path path) override;

    std::string resolve(const std::string& virtualPath) const override; 
    std::string resolveFrom(
        const std::string& virtualPath,
        const std::string& folder 
    ) const override; 
};


#endif // ENGINE_PROJECT_GENERALPATHRESOLVER_HPP_
