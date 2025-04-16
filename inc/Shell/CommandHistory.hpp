#ifndef COMMAND_HISTORY_HPP
#define COMMAND_HISTORY_HPP

#include <vector>
#include <string>

class CommandHistory {
    public:
        CommandHistory(const std::string& historyFilePath);
        ~CommandHistory();

        const std::string& getPreviousCommand();
        const std::string& getNextCommand();

    private:
        std::vector<std::string> history;
        int cursor = -1; // position actuelle
        std::string filePath;
};

#endif // COMMAND_HISTORY_HPP