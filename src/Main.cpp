#include "Shell/Shell.hpp"

int main() {
    Shell& shell = Shell::getInstance();
    shell.printProjectInfo();
    return 0;
}
