// File: Shell_Replxx.cpp

#include "Shell/Shell_Replxx.hpp"
#include "Shell/Completer.hpp"
#include <replxx.hxx>

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>

#include "Version.hpp"

namespace FusioCore {

namespace {

const std::vector<std::string> basicCommands = {
    "clear", "exit", "help", "cd", "pwd", "ls"
};

std::string formatPrompt(const std::string& message) {
    return "\033[0m" + message + "\033[0m";
}

replxx::Replxx::completions_t basic_completion_callback(const std::string& input, int& contextLen) {
    replxx::Replxx::completions_t completions;
    contextLen = input.size();
    for (const auto& cmd : basicCommands) {
        if (cmd.find(input) == 0) {
            completions.emplace_back(cmd);
        }
    }
    return completions;
}

void highlight_callback(const std::string& input, replxx::Replxx::colors_t& colors) {
    for (const auto& kw : basicCommands) {
        size_t pos = 0;
        while ((pos = input.find(kw, pos)) != std::string::npos) {
            bool isWholeWord =
                (pos == 0 || !std::isalnum(input[pos - 1])) &&
                (pos + kw.size() == input.size() || !std::isalnum(input[pos + kw.size()]));
            if (isWholeWord) {
                for (size_t i = 0; i < kw.size(); ++i) {
                    colors[pos + i] = replxx::Replxx::Color::GREEN;
                }
            }
            pos += kw.size();
        }
    }
}

} // namespace

Shell_Replxx::Shell_Replxx() {
    rx.set_highlighter_callback(highlight_callback);
    rx.set_completion_callback(basic_completion_callback);

    rx.history_load("command_history.txt");
    rx.set_max_history_size(1000);
    rx.set_unique_history(true);

    rx.set_word_break_characters(" \t\n\"\\'`@$><=;|&{}().");
    rx.set_no_color(false);
    rx.set_complete_on_empty(true);
    rx.set_double_tab_completion(true);
    rx.set_beep_on_ambiguous_completion(false);
    rx.set_immediate_completion(true);

    rx.bind_key(replxx::Replxx::KEY::control('R'),
        replxx::Replxx::key_press_handler_t{
            [](char32_t) -> replxx::Replxx::ACTION_RESULT {
                return replxx::Replxx::ACTION_RESULT::CONTINUE;
            }
        }
    );
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
    std::cout << message;
    if (newLine) {
        std::cout << "\n";
    }
    std::cout << "\033[0m";
}

void Shell_Replxx::printBold(const std::string& message, bool newLine) const {
    printBold(message, ShellType::DEFAULT, newLine);
}

void Shell_Replxx::printBold(const std::string& message, ShellType type, bool newLine) const {
    std::cout << "\033[1m";
    print(message, type, newLine);
    std::cout << "\033[0m";
}

void Shell_Replxx::printProjectInfo() const {
    printBold("=== " + std::string(FusioCore::Version::NAME) + " v" + std::string(FusioCore::Version::VERSION) + " ===", ShellType::INFO);
}

std::string Shell_Replxx::waitInput(const std::string& message) {
    std::string line;
    if (auto input = rx.input(formatPrompt(message))) {
        line = input;
        if (!line.empty()) {
            rx.history_add(line);
            rx.history_save("command_history.txt");
        }
    }
    return line;
}

} // namespace FusioCore
