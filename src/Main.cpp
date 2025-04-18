#include "Shell/Shell.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    try {
        auto& shell = FusioCore::Shell::getInstance();
        shell.printProjectInfo();
        
        while (true) {
            std::string input = shell.waitInput(">> ");
            if (input == "exit") {
                break;
            }
            // Traitement des commandes ici
            shell.print(input);
        }
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
