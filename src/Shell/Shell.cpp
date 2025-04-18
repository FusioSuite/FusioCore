#include "Shell/Shell.hpp"
#include "Version.hpp"
#include <iostream>
#include <string>

namespace FusioCore {

Shell& Shell::getInstance() {
    static Shell instance;
    return instance;
}

Shell::Shell() {
    // Initialisation si nécessaire
}

Shell::~Shell() {
    // Nettoyage si nécessaire
}

void Shell::print(const std::string& message, bool newLine) const {
    std::cout << message << (newLine ? "\n" : "") << std::flush;
}

void Shell::print(const std::string& message, ShellType type, bool newLine) const {
    std::cout << getColorCode(type) << message << resetAnsi() << (newLine ? "\n" : "") << std::flush;
}

void Shell::printBold(const std::string& message, bool newLine) const {
    std::cout << getAnsiCode({static_cast<int>(ANSI_Effect::BOLD)}) << message << resetAnsi() << (newLine ? "\n" : "") << std::flush;
}

void Shell::printBold(const std::string& message, ShellType type, bool newLine) const {
    std::cout << getAnsiCode({static_cast<int>(ANSI_Effect::BOLD)}) << getColorCode(type) << message << resetAnsi() << (newLine ? "\n" : "") << std::flush;
}

void Shell::printProjectInfo() const {
    printBold("=== " + std::string(Version::NAME) + " v" + std::string(Version::VERSION) + " ===", ShellType::INFO);
}

std::string Shell::waitInput(const std::string& message) {
    std::string input;
    std::cout << message;
    std::getline(std::cin, input);
    return input;
}

} // namespace FusioCore 