#include "Shell/Shell_Replxx.hpp"

int main() {
    FusioCore::Shell& shell = FusioCore::Shell_Replxx::getInstance();
    shell.printProjectInfo();
    while (true) {
        std::string input = shell.waitInput(">> ");
        if (input == "exit") {
            break;
        }
    }
    return 0;
}
