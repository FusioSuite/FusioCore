#ifndef VERSION_HPP
#define VERSION_HPP

#include <string>

namespace FusioCore {

// Informations sur le projet générées automatiquement par CMake
struct Version {
    // Version du projet
    static constexpr const char* VERSION = "0.1.0";
    
    // Nom du projet
    static constexpr const char* NAME = "FusioCore";
    
    // Description du projet
    static constexpr const char* DESCRIPTION = "Core software for the FusioSuite";
    
    // Auteur du projet
    static constexpr const char* AUTHOR = "FusioSuite Team";
    
    // Année de création
    static constexpr const char* YEAR = "2025";
    
    // URL du projet
    static constexpr const char* URL = "https://github.com/FusioSuite/FusioCore";
    
    // Date de compilation
    static constexpr const char* BUILD_DATE = "2025-04-17";
    
    // Compilateur utilisé
    static constexpr const char* COMPILER = "GNU";
    
    // Version du compilateur
    static constexpr const char* COMPILER_VERSION = "14.2.1";
};

} // namespace FusioCore

#endif // VERSION_HPP 
