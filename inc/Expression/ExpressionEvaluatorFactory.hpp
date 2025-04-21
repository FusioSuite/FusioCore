#ifndef EXPRESSIONEVALUATORFACTORY_HPP
#define EXPRESSIONEVALUATORFACTORY_HPP

#include "Expression/IExpressionEvaluator.hpp"
#include <memory>
#include <string>

namespace FusioCore {

/**
 * Type d'évaluateur d'expressions
 */
enum class EvaluatorType {
    EXPRTK,  // Utilise la bibliothèque ExprTk
    // Autres types d'évaluateurs peuvent être ajoutés ici
};

/**
 * Fabrique pour créer des évaluateurs d'expressions
 */
class ExpressionEvaluatorFactory {
public:
    /**
     * Crée un évaluateur d'expressions du type spécifié
     * @param type Le type d'évaluateur à créer
     * @return Un pointeur vers l'évaluateur créé
     */
    static std::shared_ptr<IExpressionEvaluator> createEvaluator(EvaluatorType type);
    
    /**
     * Crée un évaluateur d'expressions à partir de son nom
     * @param name Le nom de l'évaluateur (ex: "exprtk", "custom", etc.)
     * @return Un pointeur vers l'évaluateur créé
     * @throw std::runtime_error si le nom est inconnu
     */
    static std::shared_ptr<IExpressionEvaluator> createEvaluator(const std::string& name);
};

} // namespace FusioCore 

#endif // EXPRESSIONEVALUATORFACTORY_HPP