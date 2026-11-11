#ifndef ENGINE_CORE_COMMANDLINEARGS_HPP_
#define ENGINE_CORE_COMMANDLINEARGS_HPP_

#include <unordered_map>
#include <string>

class CommandLineArgs {
    std::unordered_map<std::string, std::string> args;
public:
    CommandLineArgs(int argc, char* argv[]);

    bool has(const std::string& key) const { return args.contains(key); }
    std::string get(const std::string& key) { return args.at(key); }
};

#endif // ENGINE_CORE_COMMANDLINEARGS_HPP_
