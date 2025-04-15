#include "Shell/Shell.hpp"

// Constructeur privé
Shell::Shell() {
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