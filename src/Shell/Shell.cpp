#include "Shell/Shell.hpp"
#include "linenoise.h"

#include <vector>

std::vector<std::string> keywords = {
    "clear", "close", "clone", "cd", "cat", "chmod"
};


void completionCallback(const char* input, linenoiseCompletions* lc) {
    std::string prefix(input);
    std::vector<std::string> matches;

    for (const auto& kw : keywords) {
        if (kw.rfind(prefix, 0) == 0) { // starts_with
            linenoiseAddCompletion(lc, kw.c_str());
            matches.push_back(kw);
        }
    }

    if (matches.empty()) {
        linenoiseAddCompletion(lc, input); // Ne complète rien, mais bloque le tab
    }
}

// Constructeur privé
Shell::Shell() {
    linenoiseHistoryLoad("command_history.txt");
    linenoiseSetCompletionCallback(completionCallback);
}

// Destructeur privé
Shell::~Shell() {
}

// Méthode pour obtenir l'instance unique (thread-safe depuis C++11)
Shell& Shell::getInstance() {
    static Shell instance;
    return instance;
}

void Shell::print(const std::string& message, bool newLine) const {
    print(message, ShellType::DEFAULT, newLine);
}

// Implémentation de la fonction print
void Shell::print(const std::string& message, ShellType type, bool newLine) const {
    switch (type) {
        case ShellType::INFO:
            std::cout << "\033[94m";
            break;
        case ShellType::WARNING:
            std::cout << "\033[93m";
            break;
        case ShellType::ERROR:
            std::cout << "\033[91m";
            break;
        case ShellType::SUCCESS:
            std::cout << "\033[92m";
            break;
        default:
            break;
    }

    std::cout << message << (newLine ? "\n" : "");
    std::cout << "\033[0m";
} 

void Shell::printBold(const std::string& message, bool newLine) const {
    printBold(message, ShellType::DEFAULT, newLine);
}

void Shell::printBold(const std::string& message, ShellType type, bool newLine) const {
    std::cout << "\033[1m";
    print(message, type, newLine);
    std::cout << "\033[0m";
}

// Implémentation de la fonction printProjectInfo
void Shell::printProjectInfo() const {
    printBold("=== " + std::string(FusioCore::Version::NAME) + " v" + std::string(FusioCore::Version::VERSION) + " ===", ShellType::INFO);
}

std::string Shell::waitInput(const std::string& message) const {
    char* line = linenoise(message.c_str());

    std::string input(line);
    free(line);

    linenoiseHistoryAdd(input.c_str());
    linenoiseHistorySave("command_history.txt");

    return input;
    // execute(input); // ⇨ ton moteur de commande
}

