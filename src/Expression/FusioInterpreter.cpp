#include "Expression/FusioInterpreter.hpp"
#include "Value/Value.hpp"
#include <sstream>
#include <algorithm>
#include <stdexcept>

namespace FusioCore {

FusioInterpreter::FusioInterpreter()
    : evaluator_(std::make_unique<ExprTkEvaluator>())
    , assignmentRegex_("^\\s*([a-zA-Z][a-zA-Z0-9_]*)\\s*=\\s*(.+)\\s*$")
    , vectorRegex_("^\\s*\\[(.+)\\]\\s*$")
    , matrixRegex_("^\\s*\\[(.+)\\]\\s*$")
{
}

FusioInterpreter::~FusioInterpreter() = default;

std::shared_ptr<IValue> FusioInterpreter::evaluate(const std::string& input) {
    // Vérifier si c'est une assignation
    if (isAssignment(input)) {
        return processAssignment(input);
    }
    
    // Vérifier si c'est une création de vecteur
    if (isVectorCreation(input)) {
        return processVectorCreation(input);
    }
    
    // Vérifier si c'est une création de matrice
    if (isMatrixCreation(input)) {
        return processMatrixCreation(input);
    }
    
    // Sinon, évaluer comme une expression normale
    return evaluator_->evaluate(input);
}

bool FusioInterpreter::isValid(const std::string& input) {
    // Vérifier si c'est une assignation valide
    if (isAssignment(input)) {
        auto [varName, expr] = parseAssignment(input);
        return evaluator_->isValid(expr);
    }
    
    // Vérifier si c'est une création de vecteur valide
    if (isVectorCreation(input)) {
        try {
            auto elements = parseVectorElements(input);
            for (const auto& element : elements) {
                if (!evaluator_->isValid(element)) {
                    return false;
                }
            }
            return true;
        } catch (...) {
            return false;
        }
    }
    
    // Vérifier si c'est une création de matrice valide
    if (isMatrixCreation(input)) {
        try {
            auto elements = parseVectorElements(input);
            for (const auto& element : elements) {
                if (!evaluator_->isValid(element)) {
                    return false;
                }
            }
            return true;
        } catch (...) {
            return false;
        }
    }
    
    // Sinon, vérifier comme une expression normale
    return evaluator_->isValid(input);
}

void FusioInterpreter::setVariable(const std::string& name, const std::shared_ptr<IValue>& value) {
    evaluator_->setVariable(name, value);
}

std::shared_ptr<IValue> FusioInterpreter::getVariable(const std::string& name) {
    return evaluator_->getVariable(name);
}

void FusioInterpreter::removeVariable(const std::string& name) {
    evaluator_->removeVariable(name);
}

void FusioInterpreter::clearVariables() {
    evaluator_->clearVariables();
}

std::vector<std::pair<std::string, std::shared_ptr<IValue>>> FusioInterpreter::listVariables() const {
    // Cette méthode nécessiterait une extension de l'interface ExprTkEvaluator
    // pour exposer les variables internes. Pour l'instant, retournons un vecteur vide.
    return {};
}

std::shared_ptr<IValue> FusioInterpreter::processAssignment(const std::string& input) {
    auto [varName, expr] = parseAssignment(input);
    
    // Évaluer l'expression
    auto result = evaluator_->evaluate(expr);
    
    // Stocker la variable
    evaluator_->setVariable(varName, result);
    
    // Retourner le résultat
    return result;
}

std::shared_ptr<IValue> FusioInterpreter::processVectorCreation(const std::string& input) {
    auto elements = parseVectorElements(input);
    
    // Évaluer chaque élément
    std::vector<double> values;
    for (const auto& element : elements) {
        auto result = evaluator_->evaluate(element);
        if (result->isScalar()) {
            values.push_back(std::dynamic_pointer_cast<Scalar>(result)->getValue());
        } else {
            throw std::runtime_error("Les éléments d'un vecteur doivent être des scalaires");
        }
    }
    
    // Créer le vecteur
    Eigen::VectorXd vectorData(values.size());
    for (size_t i = 0; i < values.size(); ++i) {
        vectorData(i) = values[i];
    }
    
    return std::make_shared<Vector>(vectorData);
}

std::shared_ptr<IValue> FusioInterpreter::processMatrixCreation(const std::string& input) {
    auto elements = parseVectorElements(input);
    
    // Déterminer les dimensions de la matrice
    auto [rows, cols] = parseMatrixDimensions(input);
    
    // Évaluer chaque élément
    std::vector<double> values;
    for (const auto& element : elements) {
        auto result = evaluator_->evaluate(element);
        if (result->isScalar()) {
            values.push_back(std::dynamic_pointer_cast<Scalar>(result)->getValue());
        } else {
            throw std::runtime_error("Les éléments d'une matrice doivent être des scalaires");
        }
    }
    
    // Vérifier que le nombre d'éléments correspond aux dimensions
    if (values.size() != rows * cols) {
        throw std::runtime_error("Le nombre d'éléments ne correspond pas aux dimensions de la matrice");
    }
    
    // Créer la matrice
    Eigen::MatrixXd matrixData(rows, cols);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            matrixData(i, j) = values[i * cols + j];
        }
    }
    
    return std::make_shared<Matrix>(matrixData);
}

bool FusioInterpreter::isAssignment(const std::string& input) const {
    std::smatch match;
    return std::regex_match(input, match, assignmentRegex_);
}

bool FusioInterpreter::isVectorCreation(const std::string& input) const {
    std::smatch match;
    return std::regex_match(input, match, vectorRegex_);
}

bool FusioInterpreter::isMatrixCreation(const std::string& input) const {
    std::smatch match;
    return std::regex_match(input, match, matrixRegex_);
}

std::pair<std::string, std::string> FusioInterpreter::parseAssignment(const std::string& input) const {
    std::smatch match;
    if (std::regex_match(input, match, assignmentRegex_)) {
        return {match[1].str(), match[2].str()};
    }
    throw std::runtime_error("Format d'assignation invalide");
}

std::vector<std::string> FusioInterpreter::parseVectorElements(const std::string& input) const {
    std::vector<std::string> elements;
    
    // Extraire le contenu entre les crochets
    std::smatch match;
    if (!std::regex_match(input, match, vectorRegex_)) {
        throw std::runtime_error("Format de vecteur invalide");
    }
    
    std::string content = match[1].str();
    
    // Analyser le contenu pour extraire les éléments
    std::istringstream iss(content);
    std::string element;
    std::string currentElement;
    
    bool inQuotes = false;
    int parenthesesCount = 0;
    
    for (char c : content) {
        if (c == '\'' && !inQuotes) {
            inQuotes = true;
            currentElement += c;
        } else if (c == '\'' && inQuotes) {
            inQuotes = false;
            currentElement += c;
        } else if (c == '(' && !inQuotes) {
            parenthesesCount++;
            currentElement += c;
        } else if (c == ')' && !inQuotes) {
            parenthesesCount--;
            currentElement += c;
        } else if ((c == ',' || c == ';') && !inQuotes && parenthesesCount == 0) {
            // Fin d'un élément
            if (!currentElement.empty()) {
                // Supprimer les espaces en début et fin
                currentElement = std::regex_replace(currentElement, std::regex("^\\s+|\\s+$"), "");
                elements.push_back(currentElement);
                currentElement.clear();
            }
        } else {
            currentElement += c;
        }
    }
    
    // Ajouter le dernier élément s'il existe
    if (!currentElement.empty()) {
        currentElement = std::regex_replace(currentElement, std::regex("^\\s+|\\s+$"), "");
        elements.push_back(currentElement);
    }
    
    return elements;
}

std::pair<size_t, size_t> FusioInterpreter::parseMatrixDimensions(const std::string& input) const {
    // Pour l'instant, nous supposons que la matrice est carrée
    // Dans une implémentation complète, nous devrions analyser la structure pour déterminer les dimensions
    auto elements = parseVectorElements(input);
    size_t size = elements.size();
    size_t dimension = static_cast<size_t>(std::sqrt(size));
    
    if (dimension * dimension != size) {
        throw std::runtime_error("La matrice doit être carrée");
    }
    
    return {dimension, dimension};
}

} // namespace FusioCore 