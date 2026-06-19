#include "CommandLineArgs.hpp"

CommandLineArgs::CommandLineArgs(int argc, char* argv[]) {
    if (argc <= 1) return;

    std::string currentArg = argv[0];
    for (int i = 1; i < argc; ++i) {
        bool isNewFlag = (std::string(argv[i]).compare(0, 2, "--") == 0);
        if (currentArg.compare(0, 2, "--") == 0 && !isNewFlag) {
            args[currentArg] = argv[i];
        }
        currentArg = std::string(argv[i]);
    }
}