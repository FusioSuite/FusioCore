#ifndef SHELL_HPP
#define SHELL_HPP

#include <iostream>
#include <memory>
#include <string>
#include "Version.hpp"

enum class ShellType {
    DEFAULT,
    INFO,
    WARNING,
    ERROR,
    SUCCESS
};

class Shell {
    public:
        // Suppression des constructeurs de copie et d'assignation
        Shell(const Shell&) = delete;
        Shell& operator=(const Shell&) = delete;
        
        // Suppression des constructeurs de déplacement et d'assignation
        Shell(Shell&&) = delete;
        Shell& operator=(Shell&&) = delete;

        // Méthode pour obtenir l'instance unique
        static Shell& getInstance();

        // Fonction pour afficher du texte dans la console
        void print(const std::string& message, bool newLine = true) const;
        void print(const std::string& message, ShellType type, bool newLine = true) const;

        void printBold(const std::string& message, bool newLine = true) const;
        void printBold(const std::string& message, ShellType type, bool newLine = true) const;

        // Fonction pour afficher les informations du projet
        void printProjectInfo() const;

    private:
        // Constructeur privé
        Shell();
        
        // Destructeur privé
        ~Shell();
};

#endif // SHELL_HPP
