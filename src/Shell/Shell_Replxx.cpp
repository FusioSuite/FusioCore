#include "Shell/Shell_Replxx.hpp"
#include <replxx.hxx>

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <cctype>

#include "Version.hpp"

namespace FusioCore {

namespace {
    const std::vector<std::string> keywords = {
        "clear", "close", "clone", "cd", "cat", "chmod"
    };

    replxx::Replxx::completions_t complete(const std::string& input, int& contextLen) {
        replxx::Replxx::completions_t out;
        std::string prefix = input.substr(0, contextLen);
        for (const auto& kw : keywords) {
            if (kw.rfind(prefix, 0) == 0) {
                out.emplace_back(kw);
            }
        }
        if (out.empty()) {
            out.emplace_back(""); // bloque la tabulation réelle
        }
        return out;
    }

    void highlight(const std::string& input, replxx::Replxx::colors_t& colors) {
        for (const auto& kw : keywords) {
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
}

replxx::Replxx rx;

Shell_Replxx::Shell_Replxx() : Shell() {
    // Configuration des callbacks
    rx.set_completion_callback(complete);
    rx.set_highlighter_callback(highlight);

    // Historique
    rx.history_load("command_history.txt");
    rx.set_max_history_size(1000);
    rx.set_unique_history(true);

    // Options de complétion
    rx.set_complete_on_empty(true);
    rx.set_double_tab_completion(false);
    rx.set_completion_count_cutoff(128);
    rx.set_beep_on_ambiguous_completion(true);

    // Couleurs & navigation
    rx.set_no_color(false);
    rx.set_word_break_characters(" \t\n~!@#$%^&*()=+[{]}\\|;:'\",<>/?");

    // Bind touche Ctrl+R
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

    std::cout << message << (newLine ? "\n" : "") << "\033[0m";
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
    if (auto input = rx.input(message)) {
        line = input;
        if (!line.empty()) {
            rx.history_add(line);
            rx.history_save("command_history.txt");
        }
    }
    return line;
}

} // namespace FusioCore