#include "Shell/Shell_Replxx.hpp"
#include <replxx.hxx>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include "Version.hpp"

namespace FusioCore {

namespace {

// Liste de commandes de base pour complétion et surlignage
const std::vector<std::string> basicCommands = {
    "clear", "exit", "help", "cd", "pwd", "ls"
};

// Surligne les mots-clés connus
void highlight_callback(const std::string& input, replxx::Replxx::colors_t& colors) {
    for (const auto& kw : basicCommands) {
        size_t pos = 0;
        while ((pos = input.find(kw, pos)) != std::string::npos) {
            bool isWholeWord =
                (pos == 0 || !std::isalnum(input[pos - 1])) &&
                (pos + kw.size() == input.size() || !std::isalnum(input[pos + kw.size()]));
            if (isWholeWord) {
                for (size_t i = 0; i < kw.size(); ++i) {
                    colors[pos + i] = replxx::Replxx::Color::BRIGHTGREEN;
                }
            }
            pos += kw.size();
        }
    }
}

} // namespace

Shell_Replxx::Shell_Replxx() {
    rx.set_highlighter_callback(highlight_callback);

    // Complétion basique via lambda
    rx.set_completion_callback([](const std::string& input, int& contextLen) -> replxx::Replxx::completions_t {
        replxx::Replxx::completions_t completions;
        contextLen = input.size();

        for (const auto& cmd : basicCommands) {
            if (cmd.find(input) == 0) {
                completions.emplace_back(cmd);
            }
        }
        return completions;
    });

    // Configuration de l'historique
    rx.set_max_history_size(1000);
    rx.set_unique_history(true);
    rx.set_word_break_characters(" \t\n\"\\'`@$><=;|&{}().");
    rx.set_no_color(false);
    rx.set_complete_on_empty(false);
    rx.history_load("command_history.txt");
}

Shell_Replxx::~Shell_Replxx() = default;

Shell_Replxx& Shell_Replxx::getInstance() {
    static Shell_Replxx instance;
    return instance;
}

void Shell_Replxx::print(const std::string& message, bool newLine) const {
    print(message, ShellType::DEFAULT, newLine);
}

void Shell_Replxx::print(const std::string& message, ShellType type, bool newLine) const {
    switch (type) {
        case ShellType::INFO:    std::cout << "\033[94m"; break;
        case ShellType::WARNING: std::cout << "\033[93m"; break;
        case ShellType::ERROR:   std::cout << "\033[91m"; break;
        case ShellType::SUCCESS: std::cout << "\033[92m"; break;
        default: break;
    }
    std::cout << message << (newLine ? "\n" : "") << "\033[0m";
}

void Shell_Replxx::printBold(const std::string& message, bool newLine) const {
    std::cout << "\033[1m";
    print(message, ShellType::DEFAULT, newLine);
    std::cout << "\033[0m";
}

void Shell_Replxx::printBold(const std::string& message, ShellType type, bool newLine) const {
    std::cout << "\033[1m";
    print(message, type, newLine);
    std::cout << "\033[0m";
}

void Shell_Replxx::printProjectInfo() const {
    printBold("=== " + std::string(FusioCore::Version::NAME) + " v" + std::string(FusioCore::Version::VERSION) + " ===", ShellType::INFO);
}

std::string Shell_Replxx::waitInput(const std::string& prompt) {
    std::string line;
    if (auto input = rx.input(prompt)) {
        line = input;
        if (!line.empty()) {
            rx.history_add(line);
            rx.history_save("command_history.txt");
        }
    }
    return line;
}

} // namespace FusioCore
