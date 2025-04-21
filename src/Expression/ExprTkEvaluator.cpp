#include "Expression/ExprTkEvaluator.hpp"
#include "Value/Scalar.hpp"
#include "Value/Vector.hpp"
#include "Value/Matrix.hpp"
#include <stdexcept>
#include <cmath>

namespace FusioCore {

ExprTkEvaluator::ExprTkEvaluator() {
    // Initialisation des symboles ExprTk
    symbolTable_.add_constants();
    expression_.register_symbol_table(symbolTable_);
}

ExprTkEvaluator::~ExprTkEvaluator() = default;

std::shared_ptr<IValue> ExprTkEvaluator::evaluate(const std::string& expression) {
    // Vérifier si c'est une variable
    auto it = variables_.find(expression);
    if (it != variables_.end()) {
        return it->second;
    }
    
    // Vérifier si l'expression est valide
    if (!isValid(expression)) {
        throw std::runtime_error("Expression invalide: " + expression);
    }
    
    // Compiler l'expression
    if (!parser_.compile(expression, expression_)) {
        throw std::runtime_error("Erreur de compilation: " + parser_.error());
    }
    
    // Évaluer l'expression
    double result = expression_.value();
    
    // Convertir le résultat en IValue
    return doubleToValue(result);
}

bool ExprTkEvaluator::isValid(const std::string& expression) {
    // Vérifier si c'est une variable
    if (variables_.find(expression) != variables_.end()) {
        return true;
    }
    return parser_.compile(expression, expression_);
}

void ExprTkEvaluator::setVariable(const std::string& name, const std::shared_ptr<IValue>& value) {
    // Stocker la variable
    variables_[name] = value;
    
    // Convertir en double pour ExprTk
    exprTkVariables_[name] = valueToDouble(value);
    
    // Mettre à jour les variables dans ExprTk
    updateExprTkVariables();
}

std::shared_ptr<IValue> ExprTkEvaluator::getVariable(const std::string& name) {
    auto it = variables_.find(name);
    if (it != variables_.end()) {
        return it->second;
    }
    return nullptr;
}

void ExprTkEvaluator::removeVariable(const std::string& name) {
    variables_.erase(name);
    exprTkVariables_.erase(name);
    updateExprTkVariables();
}

void ExprTkEvaluator::clearVariables() {
    variables_.clear();
    exprTkVariables_.clear();
    updateExprTkVariables();
}

double ExprTkEvaluator::valueToDouble(const std::shared_ptr<IValue>& value) const {
    if (!value) {
        return 0.0;
    }
    
    if (value->isScalar()) {
        auto scalar = std::dynamic_pointer_cast<Scalar>(value);
        return scalar->getValue();
    }
    
    if (value->isVector()) {
        auto vector = std::dynamic_pointer_cast<Vector>(value);
        return vector->getData().norm(); // Retourne la norme du vecteur
    }
    
    if (value->isMatrix()) {
        auto matrix = std::dynamic_pointer_cast<Matrix>(value);
        return matrix->getData().norm(); // Retourne la norme de la matrice
    }
    
    return 0.0;
}

std::shared_ptr<IValue> ExprTkEvaluator::doubleToValue(double value) const {
    return std::make_shared<Scalar>(value);
}

void ExprTkEvaluator::updateExprTkVariables() {
    // Réinitialiser la table de symboles
    symbolTable_.clear();
    symbolTable_.add_constants();
    
    // Ajouter les variables
    for (const auto& pair : exprTkVariables_) {
        symbolTable_.add_variable(pair.first, exprTkVariables_[pair.first]);
    }
    
    // Réinitialiser l'expression
    expression_.register_symbol_table(symbolTable_);
}

} // namespace FusioCore 