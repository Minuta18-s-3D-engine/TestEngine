#include "ShaderDiagnostic.hpp"

ShaderDiagnostic::Issue::Issue(
    IssueType _type, VirtualPath _file, int _line, int _column, 
    std::string _message
) : type(_type), file(_file), line(_line), column(_column), message(_message) {

}

void ShaderDiagnostic::report(const Issue& issue) {
    reportedIssues.push_back(issue);

    if (issue.type == IssueType::Critical) {
        throw exc::critical_error(issue.message);
    }
}

void ShaderDiagnostic::clear() {
    reportedIssues.clear();
}

bool ShaderDiagnostic::isEmpty() const {
    return reportedIssues.size() > 0;
}

std::vector<ShaderDiagnostic::Issue>& ShaderDiagnostic::getFullDiagnostic() {
    return reportedIssues;
}
