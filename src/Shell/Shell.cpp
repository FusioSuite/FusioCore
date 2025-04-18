#include "Shell/Shell.hpp"

namespace FusioCore {

// Implémentation du constructeur
Shell::Shell() {
    // Initialisation si nécessaire
}

// Implémentation du destructeur
Shell::~Shell() {
    // Nettoyage si nécessaire
}

// Implémentation de getInstance
Shell& Shell::getInstance() {
    // Cette méthode ne devrait jamais être appelée car Shell est une classe abstraite
    throw std::runtime_error("Shell::getInstance() ne doit pas être appelé directement");
}

} // namespace FusioCore 