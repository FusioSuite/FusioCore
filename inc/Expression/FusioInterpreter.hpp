#ifndef FUSIO_INTERPRETER_HPP
#define FUSIO_INTERPRETER_HPP

#include "Expression/IExpressionEvaluator.hpp"
#include "Expression/ExprTkEvaluator.hpp"
#include <map>
#include <string>
#include <memory>
#include <vector>
#include <regex>

namespace FusioCore {


class FusioInterpreter {
public:
    FusioInterpreter();
    ~FusioInterpreter();
    
    /**
     * Évalue une expression ou une commande
     * @param input L'entrée utilisateur à évaluer
     * @return Le résultat de l'évaluation
     */
    std::shared_ptr<IValue> evaluate(const std::string& input);
    
    /**
     * Vérifie si une expression est valide
     * @param input L'entrée à vérifier
     * @return true si l'entrée est valide, false sinon
     */
    bool isValid(const std::string& input);
    
    /**
     * Définit une variable dans l'environnement d'évaluation
     * @param name Le nom de la variable
     * @param value La valeur de la variable
     */
    void setVariable(const std::string& name, const std::shared_ptr<IValue>& value);
    
    /**
     * Obtient la valeur d'une variable
     * @param name Le nom de la variable
     * @return La valeur de la variable, ou nullptr si elle n'existe pas
     */
    std::shared_ptr<IValue> getVariable(const std::string& name);
    
    /**
     * Supprime une variable de l'environnement d'évaluation
     * @param name Le nom de la variable à supprimer
     */
    void removeVariable(const std::string& name);
    
    /**
     * Efface toutes les variables de l'environnement d'évaluation
     */
    void clearVariables();
    
    /**
     * Liste toutes les variables définies dans l'environnement
     * @return Un vecteur de paires (nom, valeur) des variables
     */
    std::vector<std::pair<std::string, std::shared_ptr<IValue>>> listVariables() const;
    
private:
    // Évaluateur ExprTk sous-jacent
    std::unique_ptr<ExprTkEvaluator> evaluator_;
    
    // Traite une assignation de variable (avec =)
    std::shared_ptr<IValue> processAssignment(const std::string& input);
    
    // Traite une création de vecteur (avec [])
    std::shared_ptr<IValue> processVectorCreation(const std::string& input);
    
    // Traite une création de matrice (avec [])
    std::shared_ptr<IValue> processMatrixCreation(const std::string& input);
    
    // Vérifie si une entrée est une assignation
    bool isAssignment(const std::string& input) const;
    
    // Vérifie si une entrée est une création de vecteur
    bool isVectorCreation(const std::string& input) const;
    
    // Vérifie si une entrée est une création de matrice
    bool isMatrixCreation(const std::string& input) const;
    
    // Extrait le nom de la variable et l'expression d'une assignation
    std::pair<std::string, std::string> parseAssignment(const std::string& input) const;
    
    // Extrait les éléments d'un vecteur ou d'une matrice
    std::vector<std::string> parseVectorElements(const std::string& input) const;
    
    // Extrait les dimensions d'une matrice
    std::pair<size_t, size_t> parseMatrixDimensions(const std::string& input) const;
    
    // Expressions régulières pour la détection des patterns
    std::regex assignmentRegex_;
    std::regex vectorRegex_;
    std::regex matrixRegex_;
    std::regex scalarMatrixOpRegex_;
    std::regex matrixScalarOpRegex_;
};

} // namespace FusioCore 

#endif // FUSIO_INTERPRETER_HPP 