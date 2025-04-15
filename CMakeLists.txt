cmake_minimum_required(VERSION 3.10)

# Nom du projet
project(FusioCore VERSION 0.1.0)

# Spécifier le standard C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ajouter les répertoires d'en-tête
include_directories(${PROJECT_SOURCE_DIR}/inc)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Définir les variables pour le fichier Version.hpp
set(PROJECT_DESCRIPTION "Core software for the FusioSuite")
set(PROJECT_AUTHOR "FusioSuite Team")
set(PROJECT_YEAR "2025")
set(PROJECT_URL "https://github.com/FusioSuite/FusioCore")

# Obtenir la date de compilation
string(TIMESTAMP CMAKE_BUILD_DATE "%Y-%m-%d")

# Générer un fichier d'en-tête avec les informations du projet
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/Version.hpp.in
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/Version.hpp
)

# Récupérer tous les fichiers sources
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Créer l'exécutable
add_executable(${PROJECT_NAME} ${SOURCES})

# Options de compilation
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Installation
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
