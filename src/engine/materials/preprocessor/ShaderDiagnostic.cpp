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

void ShaderDiagnostic::report(
    IssueType _type, VirtualPath _file, int _line, int _column, 
    std::string _message
) {
    report(Issue(_type, _file, _line, _column, _message));
}

void ShaderDiagnostic::report(
    IssueType _type, std::string _message
) {
    report(Issue(_type, "", 0, 0, _message));
    reportedIssues.back().relatedToFile = false;
}

void ShaderDiagnostic::clear() {
    reportedIssues.clear();
}

bool ShaderDiagnostic::isEmpty() const {
    return reportedIssues.empty();
}

std::vector<ShaderDiagnostic::Issue>& ShaderDiagnostic::getFullDiagnostic() {
    return reportedIssues;
}

void ShaderDiagnostic::dumpToLogs() {
    for (auto& issue : reportedIssues) {
        if (!issue.relatedToFile) {
            std::cerr << issueMapper.toString(issue.type) << ": " << 
                issue.message << std::endl;
            continue;
        }

        std::cerr << issueMapper.toString(issue.type) << ": in file " << 
            issue.file.getVirtual() << ": " << issue.message << std::endl;
    }
}
