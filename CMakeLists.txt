cmake_minimum_required(VERSION 3.10)

# Nom du projet et version
project(FusioCore VERSION 0.1.0)

# Inclure FetchContent pour gérer les dépendances externes
include(FetchContent)

# Télécharger et configurer Eigen depuis ton fork GitHub
FetchContent_Declare(
    Eigen
    GIT_REPOSITORY https://github.com/FusioSuite/fusio-eigen.git
    GIT_TAG        3.4.0
)
FetchContent_MakeAvailable(Eigen)

FetchContent_Declare(
    ExprTk
    GIT_REPOSITORY https://github.com/GuyGnole/exprtk.git
    GIT_TAG        master  # ou un tag/commit spécifique
)
FetchContent_MakeAvailable(ExprTk)

# Spécifier le standard C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Générer compile_commands.json pour le linter
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Définir les variables pour le fichier Version.hpp
set(PROJECT_DESCRIPTION "Core software for the FusioSuite")
set(PROJECT_AUTHOR "FusioSuite Team")
set(PROJECT_YEAR "2025")
set(PROJECT_URL "https://github.com/FusioSuite/FusioCore")
string(TIMESTAMP CMAKE_BUILD_DATE "%Y-%m-%d")

# Trouver le module Threads
find_package(Threads REQUIRED)

# Générer un fichier d'en-tête avec les informations du projet
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/Version.hpp.in
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/Version.hpp
)

# Récupérer tous les fichiers sources
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Créer l'exécutable
add_executable(${PROJECT_NAME} ${SOURCES})

# Ajouter les répertoires d'en-tête
target_include_directories(${PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/inc
    ${CMAKE_CURRENT_BINARY_DIR}
    ${eigen_SOURCE_DIR}
    ${exprtk_SOURCE_DIR}
)

# Exporter les chemins d'inclusion pour le linter
set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/inc"
)

# # Lier les bibliothèques externes
# target_link_libraries(${PROJECT_NAME} PRIVATE 
#     Threads::Threads
# )

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
