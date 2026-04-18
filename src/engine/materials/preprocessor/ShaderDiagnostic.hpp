#ifndef ENGINE_MATERIALS_PREPROCESSOR_SHADERDIAGNOSTIC_H_
#define ENGINE_MATERIALS_PREPROCESSOR_SHADERDIAGNOSTIC_H_

#include <string>
#include <vector>
#include <iostream>
#include <exception>

#include "../../project/VirtualPath.hpp"
#include "../../stringProcessing/utils/StringInfo.hpp"
#include "../../utils/exc/GeneralExceptions.hpp"
#include "../../utils/EnumMapper.hpp"

class ShaderDiagnostic {
public:
    class preprocessor_error : public std::exception {
        std::string message;
        ShaderDiagnostic& diagnostic;
    public:
        preprocessor_error(
            const char* _message, ShaderDiagnostic diagnostic
        ) : message(_message), diagnostic(diagnostic) {}
        preprocessor_error(
            const std::string& _message, ShaderDiagnostic diagnostic
        ) : message(_message), diagnostic(diagnostic) {}

        const char* what() const noexcept { return message.c_str(); }
        ShaderDiagnostic getDiagnostic() const noexcept { return diagnostic; }
    };

    enum class IssueType {
        Note,
        Warning,
        Error,
        Critical
    }; 
    const EnumMapper<IssueType> issueMapper = {
        { IssueType::Note, "Note" },
        { IssueType::Warning, "Warning" },
        { IssueType::Error, "Error" },
        { IssueType::Critical, "Bool" }
    }; 

    struct Issue {
        IssueType type;
        VirtualPath file;
        int line, column;
        std::string message;
        bool relatedToFile = true;

        Issue(
            IssueType _type, VirtualPath _file, int _line, int _column, 
            std::string _message
        );
    };
private:
    std::vector<Issue> reportedIssues;
public:
    ShaderDiagnostic() = default;

    void report(const Issue& issue);
    void report(
        IssueType _type, VirtualPath _file, int _line, int _column, 
        std::string _message
    );
    void report(
        IssueType _type, std::string _message
    );
    void clear();

    bool isEmpty() const;
    std::vector<Issue>& getFullDiagnostic();

    void dumpToLogs();
};

#endif // ENGINE_MATERIALS_PREPROCESSOR_SHADERDIAGNOSTIC_H_
