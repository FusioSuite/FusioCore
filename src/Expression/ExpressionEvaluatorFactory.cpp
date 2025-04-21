#include "Expression/ExpressionEvaluatorFactory.hpp"
#include "Expression/ExprTkEvaluator.hpp"
#include <stdexcept>

namespace FusioCore {

std::shared_ptr<IExpressionEvaluator> ExpressionEvaluatorFactory::createEvaluator(EvaluatorType type) {
    switch (type) {
        case EvaluatorType::EXPRTK:
            return std::make_shared<ExprTkEvaluator>();
        default:
            throw std::runtime_error("Type d'évaluateur non supporté");
    }
}

std::shared_ptr<IExpressionEvaluator> ExpressionEvaluatorFactory::createEvaluator(const std::string& name) {
    if (name == "exprtk") {
        return createEvaluator(EvaluatorType::EXPRTK);
    }
    
    throw std::runtime_error("Évaluateur inconnu: " + name);
}

} // namespace FusioCore 