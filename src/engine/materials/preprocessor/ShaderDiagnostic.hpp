#ifndef ENGINE_MATERIALS_PREPROCESSOR_SHADERDIAGNOSTIC_H_
#define ENGINE_MATERIALS_PREPROCESSOR_SHADERDIAGNOSTIC_H_

#include <string>
#include <vector>
#include <iostream>

#include "VirtualPath.hpp"
#include "../../stringProcessing/utils/StringInfo.hpp"
#include "../../utils/exc/GeneralExceptions.hpp"

class ShaderDiagnostic {
public:
    enum class IssueType {
        Note,
        Warning,
        Error,
        Critical
    }; 

    struct Issue {
        IssueType type;
        VirtualPath file;
        int line, column;
        std::string message;

        Issue(
            IssueType _type, VirtualPath _file, int _line, int _column, 
            std::string _message
        );
    };
private:
    std::vector<Issue> reportedIssues;
public:
    void report(const Issue& issue);
    void clear();

    bool isEmpty() const;
    std::vector<Issue>& getFullDiagnostic();
};

#endif // ENGINE_MATERIALS_PREPROCESSOR_SHADERDIAGNOSTIC_H_
