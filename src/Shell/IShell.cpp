#include "Shell/IShell.hpp"

namespace FusioCore {

// Implémentation du constructeur
IShell::IShell() {
    // Initialisation si nécessaire
}

// Implémentation du destructeur
IShell::~IShell() {
    // Nettoyage si nécessaire
}

// Implémentation de getInstance
IShell& IShell::getInstance() {
    // Cette méthode ne devrait jamais être appelée car Shell est une classe abstraite
    throw std::runtime_error("IShell::getInstance() ne doit pas être appelé directement");
}

} // namespace FusioCore 