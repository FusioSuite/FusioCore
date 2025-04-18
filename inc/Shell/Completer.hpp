// File: Completer.hpp

#pragma once

#include <string>
#include <vector>
#include <map>

namespace FusioCore {

class Completer {
public:
    enum class CompletionType {
        COMMAND,
        OPTION,
        FILE,
        VARIABLE
    };

    struct Suggestion {
        std::string text;
        CompletionType type;

        Suggestion(const std::string& t, CompletionType ct)
            : text(t), type(ct) {}
    };

    Completer();

    std::vector<Suggestion> getSuggestions(const std::string& input, int& contextLen) const;

    void addCommand(const std::string& command);
    void addCommandOptions(const std::string& command, const std::vector<std::string>& options);

private:
    std::vector<std::string> commands;
    std::map<std::string, std::vector<std::string>> commandOptions;

    std::vector<Suggestion> completeCommands(const std::string& prefix) const;
    std::vector<Suggestion> completeOptions(const std::string& command, const std::string& prefix) const;
};

} // namespace FusioCore
