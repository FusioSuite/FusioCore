#include "Shell/CommandHistory.hpp"
#include <fstream>

CommandHistory::CommandHistory(const std::string& historyFilePath) : filePath(historyFilePath) {
    std::ifstream file(filePath);
    std::string line;
    while (std::getline(file, line)) {
        history.push_back(line);
    }
    file.close();
}

CommandHistory::~CommandHistory() {
}

const std::string& CommandHistory::getPreviousCommand() {
    if (cursor > 0) {
        cursor--;
    }
    return history[cursor];
}

const std::string& CommandHistory::getNextCommand() {
    if (cursor < history.size() - 1) {
        cursor++;
    }
    return history[cursor];
}
