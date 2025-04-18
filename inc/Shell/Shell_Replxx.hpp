#ifndef SHELL_REPLXX_HPP
#define SHELL_REPLXX_HPP

#include "Shell.hpp"
#include <replxx.hxx>

namespace FusioCore {

class Shell_Replxx : public Shell {
    private:
        // Constructeur privé
        Shell_Replxx();
        
        // Supprimer les constructeurs de copie et d'affectation
        Shell_Replxx(const Shell_Replxx&) = delete;
        Shell_Replxx& operator=(const Shell_Replxx&) = delete;
        
    public:
        // Destructeur
        ~Shell_Replxx() override;
        
        // Obtenir l'instance unique
        static Shell_Replxx& getInstance();
        
        // Implémentation des méthodes virtuelles
        void print(const std::string& message, bool newLine = true) const override;
        void print(const std::string& message, ShellType type, bool newLine = true) const override;
        
        void printBold(const std::string& message, bool newLine = true) const override;
        void printBold(const std::string& message, ShellType type, bool newLine = true) const override;
        
        void printProjectInfo() const override;
        
        std::string waitInput(const std::string& message) override;
};

} // namespace FusioCore

#endif // SHELL_REPLXX_HPP