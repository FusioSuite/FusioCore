#pragma once

#include "Shell/IShell.hpp"
#include "Shell/ANSI.hpp"
#include <memory>

namespace FusioCore {

class Shell : public IShell {
public:
    static Shell& getInstance();

    // Constructeur et destructeur
    Shell();
    ~Shell() override;

    // Implémentation des méthodes d'affichage
    void print(const std::string& message, bool newLine = true) const override;
    void print(const std::string& message, ShellType type, bool newLine = true) const override;
    void printBold(const std::string& message, bool newLine = true) const override;
    void printBold(const std::string& message, ShellType type, bool newLine = true) const override;
    void printProjectInfo() const override;

    // Implémentation de la méthode d'entrée
    std::string waitInput(const std::string& message = "") override;

protected:
    // Méthodes utilitaires
    std::string getColorCode(ShellType type) const {
        switch (type) {
            case ShellType::INFO: return getAnsiCode(ANSI_Effect::BOLD, ANSI_Color::FG_BLUE);
            case ShellType::WARNING: return getAnsiCode(ANSI_Effect::BOLD, ANSI_Color::FG_YELLOW);
            case ShellType::ERROR: return getAnsiCode(ANSI_Effect::BOLD, ANSI_Color::FG_RED);
            case ShellType::SUCCESS: return getAnsiCode(ANSI_Effect::BOLD, ANSI_Color::FG_GREEN);
            default: return resetAnsi();
        }
    }

private:
    // Interdiction de la copie (déjà géré par IShell)
    using IShell::IShell;
};

} // namespace FusioCore
