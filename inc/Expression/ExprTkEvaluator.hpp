#ifndef EXPRTK_EVALUATOR_HPP
#define EXPRTK_EVALUATOR_HPP

#include "Expression/IExpressionEvaluator.hpp"
#include <map>
#include <string>
#include <memory>
#include <exprtk.hpp>

namespace FusioCore {

/**
 * Implémentation de l'évaluateur d'expressions utilisant ExprTk
 */
class ExprTkEvaluator : public IExpressionEvaluator {
public:
    ExprTkEvaluator();
    ~ExprTkEvaluator() override;
    
    std::shared_ptr<IValue> evaluate(const std::string& expression) override;
    bool isValid(const std::string& expression) override;
    void setVariable(const std::string& name, const std::shared_ptr<IValue>& value) override;
    std::shared_ptr<IValue> getVariable(const std::string& name) override;
    void removeVariable(const std::string& name) override;
    void clearVariables() override;
    
private:
    // Convertit un IValue en double pour ExprTk
    double valueToDouble(const std::shared_ptr<IValue>& value) const;
    
    // Convertit un double en IValue (Scalar)
    std::shared_ptr<IValue> doubleToValue(double value) const;
    
    // Mise à jour des variables dans l'environnement ExprTk
    void updateExprTkVariables();
    
    // Symboles ExprTk
    exprtk::symbol_table<double> symbolTable_;
    exprtk::expression<double> expression_;
    exprtk::parser<double> parser_;
    
    // Variables stockées
    std::map<std::string, std::shared_ptr<IValue>> variables_;
    
    // Variables ExprTk
    std::map<std::string, double> exprTkVariables_;
};

} // namespace FusioCore 

#endif // EXPRTK_EVALUATOR_HPP