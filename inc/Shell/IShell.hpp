#ifndef ISHELL_HPP
#define ISHELL_HPP

#include <string>
#include <iostream>

namespace FusioCore {
enum class ShellType {
    DEFAULT,
    INFO,
    WARNING,
    ERROR,
    SUCCESS
};

class IShell {
protected:
    // Constructeur et destructeur protégés pour permettre l'héritage
    IShell();
    virtual ~IShell();
    
private:
    // Supprimer les constructeurs de copie et d'affectation
    IShell(const IShell&) = delete;
    IShell& operator=(const IShell&) = delete;
    
public:
    // Obtenir l'instance unique
    static IShell& getInstance();
    
    // Méthodes pour afficher du texte
    virtual void print(const std::string& message, bool newLine = true) const = 0;
    virtual void print(const std::string& message, ShellType type, bool newLine = true) const = 0;
    
    virtual void printBold(const std::string& message, bool newLine = true) const = 0;
    virtual void printBold(const std::string& message, ShellType type, bool newLine = true) const = 0;
    
    // Afficher les informations du projet
    virtual void printProjectInfo() const = 0;
    
    // Attendre l'entrée utilisateur - maintenant non-const
    virtual std::string waitInput(const std::string& message) = 0;
};

} // namespace FusioCore

#endif // SHELL_HPP