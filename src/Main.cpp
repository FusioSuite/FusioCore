#include "Expression/FusioInterpreter.hpp"
#include "Shell/Shell.hpp"
#include "Value/Value.hpp"
#include "Expression/ExpressionEvaluatorFactory.hpp"

#include <iostream>
#include <string>

int main() {
    auto& shell = FusioCore::Shell::getInstance();
    auto interpreter = std::make_unique<FusioCore::FusioInterpreter>();
    
    shell.print("Bienvenue dans l'interpréteur de FusioCore !", FusioCore::ShellType::INFO);
    shell.print("Tapez 'exit' ou 'quit' pour quitter.", FusioCore::ShellType::INFO);
    shell.print("", FusioCore::ShellType::INFO);
    
    std::string input;
    while (true) {
        shell.print(">> ", FusioCore::ShellType::INFO, false);
        input = shell.waitInput();
        
        if (input == "exit" || input == "quit") {
            break;
        }
        
        try {
            auto result = interpreter->evaluate(input);
            shell.print(result->toString(), FusioCore::ShellType::SUCCESS);
        } catch (const std::exception& e) {
            shell.print("Erreur : " + std::string(e.what()), FusioCore::ShellType::ERROR);
        }
    }
    
    return 0;
}
