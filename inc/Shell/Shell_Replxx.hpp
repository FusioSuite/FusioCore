#pragma once

#include "Shell/Shell.hpp"
#include <replxx.hxx>
#include <memory>
#include <string>

#include "Shell/Completer.hpp"

namespace FusioCore {

class Completer;

/**
 * @brief Implémentation du Shell interactif utilisant Replxx
 */
class Shell_Replxx : public Shell {
private:
    // Constructeur privé pour singleton
    Shell_Replxx();

    // Interdit la copie
    Shell_Replxx(const Shell_Replxx&) = delete;
    Shell_Replxx& operator=(const Shell_Replxx&) = delete;

    // Moteur de ligne de commande
    replxx::Replxx rx;

    // Compléteur intelligent
    std::unique_ptr<Completer> completer;

    // Callback de complétion
    static replxx::Replxx::completions_t completion_callback(const std::string& input, int& contextLen);

public:
    ~Shell_Replxx() override;

    static Shell_Replxx& getInstance();

    // Implémentation des méthodes de Shell
    void print(const std::string& message, bool newLine = true) const override;
    void print(const std::string& message, ShellType type, bool newLine = true) const override;
    void printBold(const std::string& message, bool newLine = true) const override;
    void printBold(const std::string& message, ShellType type, bool newLine = true) const override;
    void printProjectInfo() const override;
    std::string waitInput(const std::string& message) override;
};

} // namespace FusioCore
