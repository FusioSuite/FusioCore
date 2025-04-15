#!/bin/bash

# Couleurs pour une meilleure lisibilité
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${YELLOW}Démarrage de l'analyse avec clang-tidy...${NC}"

# Vérifier si clang-tidy est installé
if ! command -v clang-tidy &> /dev/null; then
    echo -e "${RED}Erreur: clang-tidy n'est pas installé.${NC}"
    echo "Installez-le avec: sudo dnf install clang-tools-extra"
    exit 1
fi

# Créer un fichier temporaire pour la compilation database
echo -e "${YELLOW}Génération de la compilation database...${NC}"
mkdir -p build
cd build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
cd ..

# Analyser les fichiers sources
echo -e "${YELLOW}Analyse des fichiers sources...${NC}"
find src inc -name "*.cpp" -o -name "*.hpp" -o -name "*.h" | while read -r file; do
    echo -e "${GREEN}Analyse de $file${NC}"
    clang-tidy "$file" -p build
done

echo -e "${GREEN}Analyse terminée !${NC}" 