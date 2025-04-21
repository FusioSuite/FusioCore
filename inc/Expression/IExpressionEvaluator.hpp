#ifndef IEXPRESSIONEVALUATOR_HPP
#define IEXPRESSIONEVALUATOR_HPP

#include <string>
#include <memory>
#include "Value/Value.hpp"

namespace FusioCore {

/**
 * Interface pour l'évaluation d'expressions mathématiques
 */
class IExpressionEvaluator {
public:
    virtual ~IExpressionEvaluator() = default;
    
    /**
     * Évalue une expression mathématique
     * @param expression L'expression à évaluer
     * @return Le résultat de l'évaluation
     * @throw std::runtime_error si l'expression est invalide
     */
    virtual std::shared_ptr<IValue> evaluate(const std::string& expression) = 0;
    
    /**
     * Vérifie si une expression est valide
     * @param expression L'expression à vérifier
     * @return true si l'expression est valide, false sinon
     */
    virtual bool isValid(const std::string& expression) = 0;
    
    /**
     * Définit une variable dans l'environnement d'évaluation
     * @param name Le nom de la variable
     * @param value La valeur de la variable
     */
    virtual void setVariable(const std::string& name, const std::shared_ptr<IValue>& value) = 0;
    
    /**
     * Obtient la valeur d'une variable
     * @param name Le nom de la variable
     * @return La valeur de la variable, ou nullptr si elle n'existe pas
     */
    virtual std::shared_ptr<IValue> getVariable(const std::string& name) = 0;
    
    /**
     * Supprime une variable de l'environnement d'évaluation
     * @param name Le nom de la variable à supprimer
     */
    virtual void removeVariable(const std::string& name) = 0;
    
    /**
     * Efface toutes les variables de l'environnement d'évaluation
     */
    virtual void clearVariables() = 0;
};

} // namespace FusioCore 

#endif // IEXPRESSIONEVALUATOR_HPP