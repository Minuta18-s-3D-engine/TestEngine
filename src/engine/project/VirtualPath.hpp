#ifndef ENGINE_PROJECT_VIRTUAL_PATH_HPP_
#define ENGINE_PROJECT_VIRTUAL_PATH_HPP_

#include <string>
#include <filesystem>
#include <functional>

class VirtualPath {
    using ResolverFunc = std::function<std::string(const std::string&)>;
    static ResolverFunc resolver;

    std::string path;
public:
    VirtualPath();
    VirtualPath(const std::string& _path);
    VirtualPath(const char* _path);
    VirtualPath(const std::filesystem::path& _path);

    std::string resolve() const;
    const std::string& getVirtual() const { return path; }

    static void setResolverFunc(ResolverFunc func) { resolver = func; }
};

#endif // ENGINE_PROJECT_VIRTUAL_PATH_HPP_
