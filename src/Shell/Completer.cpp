// File: Completer.cpp

#include "Shell/Completer.hpp"
#include <algorithm>
#include <sstream>

namespace FusioCore {

Completer::Completer() {
    // Pré-remplir avec quelques commandes de base
    addCommand("help");
    addCommand("exit");
    addCommand("clear");
    addCommand("cd");
    addCommand("pwd");
    addCommand("ls");

    addCommandOptions("ls", {"-l", "-a", "-h", "-R", "--color"});
    addCommandOptions("cd", {"..", "/", "~"});
}

void Completer::addCommand(const std::string& command) {
    commands.push_back(command);
}

void Completer::addCommandOptions(const std::string& command, const std::vector<std::string>& options) {
    commandOptions[command] = options;
}

std::vector<Completer::Suggestion> Completer::getSuggestions(const std::string& input, int& contextLen) const {
    std::vector<Suggestion> results;
    size_t spacePos = input.find_last_of(' ');

    if (spacePos == std::string::npos) {
        // Suggère des commandes
        results = completeCommands(input);
        contextLen = input.length();
    } else {
        std::string cmd = input.substr(0, spacePos);
        std::string arg = input.substr(spacePos + 1);
        auto optResults = completeOptions(cmd, arg);
        if (!optResults.empty()) {
            results = std::move(optResults);
            contextLen = input.length();
        }
    }

    return results;
}

std::vector<Completer::Suggestion> Completer::completeCommands(const std::string& prefix) const {
    std::vector<Suggestion> out;
    for (const auto& cmd : commands) {
        if (cmd.find(prefix) == 0) {
            out.emplace_back(cmd, CompletionType::COMMAND);
        }
    }
    return out;
}

std::vector<Completer::Suggestion> Completer::completeOptions(const std::string& command, const std::string& prefix) const {
    std::vector<Suggestion> out;
    auto it = commandOptions.find(command);
    if (it != commandOptions.end()) {
        for (const auto& opt : it->second) {
            if (opt.find(prefix) == 0) {
                out.emplace_back(opt, CompletionType::OPTION);
            }
        }
    }
    return out;
}

} // namespace FusioCore
