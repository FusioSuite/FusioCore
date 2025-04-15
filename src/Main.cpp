#include "Shell/Shell.hpp"

int main() {
    Shell& shell = Shell::getInstance();
    shell.printProjectInfo();
    while (true) {
        std::string input = shell.waitInput(">> ");
        if (input == "exit") {
            break;
        }
        shell.log(input);
    }
    shell.clearLogs();
    return 0;
}
