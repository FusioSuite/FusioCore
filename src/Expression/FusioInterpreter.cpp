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
    , scalarMatrixOpRegex_("^\\s*([0-9.]+)\\s*\\*\\s*([a-zA-Z][a-zA-Z0-9_]*)\\s*$")
    , matrixScalarOpRegex_("^\\s*([a-zA-Z][a-zA-Z0-9_]*)\\s*\\*\\s*([0-9.]+)\\s*$")
{
}

FusioInterpreter::~FusioInterpreter() = default;

std::shared_ptr<IValue> FusioInterpreter::evaluate(const std::string& input) {
    // Vérifier si c'est une variable
    auto value = evaluator_->getVariable(input);
    if (value) {
        return value;
    }
    
    // Si l'entrée est un identifiant valide (lettre suivie de lettres/chiffres), c'est une variable non définie
    if (std::regex_match(input, std::regex("^[a-zA-Z][a-zA-Z0-9_]*$"))) {
        throw std::runtime_error("Variable non définie : " + input);
    }
    
    // Vérifier si c'est une fonction mathématique sur une matrice
    std::regex funcMatrixRegex("^(sin|cos|tan|exp|log|log10|sqrt)\\s*\\(([a-zA-Z][a-zA-Z0-9_]*)\\)$");
    std::smatch funcMatrixMatch;
    if (std::regex_match(input, funcMatrixMatch, funcMatrixRegex)) {
        std::string funcName = funcMatrixMatch[1].str();
        std::string matrixName = funcMatrixMatch[2].str();
        
        auto matrixValue = evaluator_->getVariable(matrixName);
        if (matrixValue && matrixValue->isMatrix()) {
            auto matrix = std::dynamic_pointer_cast<Matrix>(matrixValue);
            Eigen::MatrixXd result = matrix->getData();
            
            // Appliquer la fonction élément par élément
            for(int i = 0; i < result.rows(); ++i) {
                for(int j = 0; j < result.cols(); ++j) {
                    if(funcName == "sin") result(i,j) = std::sin(result(i,j));
                    else if(funcName == "cos") result(i,j) = std::cos(result(i,j));
                    else if(funcName == "tan") result(i,j) = std::tan(result(i,j));
                    else if(funcName == "exp") result(i,j) = std::exp(result(i,j));
                    else if(funcName == "log") result(i,j) = std::log(result(i,j));
                    else if(funcName == "log10") result(i,j) = std::log10(result(i,j));
                    else if(funcName == "sqrt") result(i,j) = std::sqrt(result(i,j));
                }
            }
            return std::make_shared<Matrix>(result);
        }
    }
    
    // Vérifier si c'est une opération scalaire * matrice
    std::smatch scalarMatrixMatch;
    if (std::regex_match(input, scalarMatrixMatch, scalarMatrixOpRegex_)) {
        double scalarValue = std::stod(scalarMatrixMatch[1].str());
        std::string matrixName = scalarMatrixMatch[2].str();
        
        auto matrixValue = evaluator_->getVariable(matrixName);
        if (matrixValue && matrixValue->isMatrix()) {
            auto matrix = std::dynamic_pointer_cast<Matrix>(matrixValue);
            return std::make_shared<Matrix>(matrix->getData() * scalarValue);
        }
    }
    
    // Vérifier si c'est une opération matrice * scalaire
    std::smatch matrixScalarMatch;
    if (std::regex_match(input, matrixScalarMatch, matrixScalarOpRegex_)) {
        std::string matrixName = matrixScalarMatch[1].str();
        double scalarValue = std::stod(matrixScalarMatch[2].str());
        
        auto matrixValue = evaluator_->getVariable(matrixName);
        if (matrixValue && matrixValue->isMatrix()) {
            auto matrix = std::dynamic_pointer_cast<Matrix>(matrixValue);
            return std::make_shared<Matrix>(matrix->getData() * scalarValue);
        }
    }
    
    // Vérifier si c'est une assignation
    if (isAssignment(input)) {
        return processAssignment(input);
    }
    
    // Vérifier si c'est une création de matrice (doit être vérifié avant le vecteur)
    if (isMatrixCreation(input)) {
        return processMatrixCreation(input);
    }
    
    // Vérifier si c'est une création de vecteur
    if (isVectorCreation(input)) {
        return processVectorCreation(input);
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
    
    // Vérifier si l'expression est une création de matrice ou de vecteur
    if (isMatrixCreation(expr)) {
        auto result = processMatrixCreation(expr);
        evaluator_->setVariable(varName, result);
        return result;
    }
    
    if (isVectorCreation(expr)) {
        auto result = processVectorCreation(expr);
        evaluator_->setVariable(varName, result);
        return result;
    }
    
    // Sinon, évaluer comme une expression normale
    auto result = evaluator_->evaluate(expr);
    evaluator_->setVariable(varName, result);
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
    // Extraire le contenu entre les crochets
    std::smatch match;
    if (!std::regex_match(input, match, matrixRegex_)) {
        throw std::runtime_error("Format de matrice invalide");
    }
    
    std::string content = match[1].str();
    
    // Diviser le contenu en lignes
    std::vector<std::string> rows;
    std::string currentRow;
    
    for (size_t i = 0; i < content.length(); ++i) {
        char c = content[i];
        
        if (c == ';') {
            if (!currentRow.empty()) {
                rows.push_back(currentRow);
                currentRow.clear();
            }
        } else {
            currentRow += c;
        }
    }
    
    // Ajouter la dernière ligne
    if (!currentRow.empty()) {
        rows.push_back(currentRow);
    }
    
    if (rows.empty()) {
        throw std::runtime_error("Matrice vide");
    }
    
    // Déterminer les dimensions
    size_t numRows = rows.size();
    size_t numCols = 0;
    
    // Compter les colonnes dans la première ligne
    std::string firstRow = rows[0];
    std::string currentElement;
    
    for (size_t i = 0; i < firstRow.length(); ++i) {
        char c = firstRow[i];
        
        if (c == ' ' || c == ',') {
            if (!currentElement.empty()) {
                numCols++;
                currentElement.clear();
            }
        } else {
            currentElement += c;
        }
    }
    
    // Ajouter le dernier élément de la première ligne
    if (!currentElement.empty()) {
        numCols++;
    }
    
    if (numCols == 0) {
        throw std::runtime_error("Matrice invalide : aucune colonne détectée");
    }
    
    // Extraire tous les éléments
    std::vector<double> values;
    
    for (const auto& row : rows) {
        std::string currentElement;
        
        for (size_t i = 0; i < row.length(); ++i) {
            char c = row[i];
            
            if (c == ' ' || c == ',') {
                if (!currentElement.empty()) {
                    auto result = evaluator_->evaluate(currentElement);
                    if (result->isScalar()) {
                        values.push_back(std::dynamic_pointer_cast<Scalar>(result)->getValue());
                    } else {
                        throw std::runtime_error("Les éléments d'une matrice doivent être des scalaires");
                    }
                    currentElement.clear();
                }
            } else {
                currentElement += c;
            }
        }
        
        // Ajouter le dernier élément de la ligne
        if (!currentElement.empty()) {
            auto result = evaluator_->evaluate(currentElement);
            if (result->isScalar()) {
                values.push_back(std::dynamic_pointer_cast<Scalar>(result)->getValue());
            } else {
                throw std::runtime_error("Les éléments d'une matrice doivent être des scalaires");
            }
        }
    }
    
    // Vérifier que le nombre d'éléments correspond aux dimensions
    if (values.size() != numRows * numCols) {
        throw std::runtime_error("Le nombre d'éléments (" + std::to_string(values.size()) + 
                                ") ne correspond pas aux dimensions de la matrice (" + 
                                std::to_string(numRows) + "x" + std::to_string(numCols) + ")");
    }
    
    // Créer la matrice
    Eigen::MatrixXd matrixData(numRows, numCols);
    for (size_t i = 0; i < numRows; ++i) {
        for (size_t j = 0; j < numCols; ++j) {
            matrixData(i, j) = values[i * numCols + j];
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
    if (!std::regex_match(input, match, vectorRegex_)) {
        return false;
    }
    
    // Vérifier que ce n'est pas une matrice
    std::string content = match[1].str();
    return content.find(';') == std::string::npos;
}

bool FusioInterpreter::isMatrixCreation(const std::string& input) const {
    std::smatch match;
    if (!std::regex_match(input, match, matrixRegex_)) {
        return false;
    }
    
    // Vérifier si le contenu contient au moins un point-virgule
    std::string content = match[1].str();
    return content.find(';') != std::string::npos;
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
    std::string currentElement;
    bool inQuotes = false;
    int parenthesesCount = 0;
    
    for (size_t i = 0; i < content.length(); ++i) {
        char c = content[i];
        
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
        } else if ((c == ' ' || c == ';' || c == ',') && !inQuotes && parenthesesCount == 0) {
            // Fin d'un élément si on n'est pas déjà dans un espace
            if (!currentElement.empty() && currentElement.back() != ' ') {
                // Supprimer les espaces en début et fin
                currentElement = std::regex_replace(currentElement, std::regex("^\\s+|\\s+$"), "");
                if (!currentElement.empty()) {
                    elements.push_back(currentElement);
                }
                currentElement.clear();
            }
        } else {
            currentElement += c;
        }
    }
    
    // Ajouter le dernier élément s'il existe
    if (!currentElement.empty()) {
        currentElement = std::regex_replace(currentElement, std::regex("^\\s+|\\s+$"), "");
        if (!currentElement.empty()) {
            elements.push_back(currentElement);
        }
    }
    
    return elements;
}

std::pair<size_t, size_t> FusioInterpreter::parseMatrixDimensions(const std::string& input) const {
    // Extraire le contenu entre les crochets
    std::smatch match;
    if (!std::regex_match(input, match, matrixRegex_)) {
        throw std::runtime_error("Format de matrice invalide");
    }
    
    std::string content = match[1].str();
    
    // Compter le nombre de points-virgules pour déterminer le nombre de lignes
    size_t rows = 1;
    for (char c : content) {
        if (c == ';') {
            rows++;
        }
    }
    
    // Compter le nombre d'éléments dans la première ligne
    size_t cols = 0;
    bool inFirstRow = true;
    std::string currentElement;
    
    for (size_t i = 0; i < content.length(); ++i) {
        char c = content[i];
        
        if (c == ';') {
            inFirstRow = false;
            currentElement.clear();
        } else if ((c == ' ' || c == ',') && inFirstRow) {
            if (!currentElement.empty()) {
                cols++;
                currentElement.clear();
            }
        } else if (inFirstRow) {
            currentElement += c;
        }
    }
    
    // Ajouter le dernier élément de la première ligne
    if (!currentElement.empty() && inFirstRow) {
        cols++;
    }
    
    if (cols == 0) {
        throw std::runtime_error("Matrice invalide : aucune colonne détectée");
    }
    
    return {rows, cols};
}

} // namespace FusioCore 