#ifndef ANSI_HPP
#define ANSI_HPP

#include <string>
#include <initializer_list>
#include <sstream>

enum class ANSI_Effect {
    NORMAL = 0,
    BOLD = 1,
    FAINT = 2,
    ITALIC = 3,
    UNDERLINE = 4,
    REVERSE = 7,
    CONCEAL = 8,
    CROSSED_OUT = 9,
};

enum class ANSI_Color {
    FG_BLACK = 30,
    FG_RED = 31,
    FG_GREEN = 32,
    FG_YELLOW = 33,
    FG_BLUE = 34,
    FG_MAGENTA = 35,
    FG_CYAN = 36,
    FG_WHITE = 37,

    FG_BRIGHT_BLACK = 90,
    FG_BRIGHT_RED = 91,
    FG_BRIGHT_GREEN = 92,
    FG_BRIGHT_YELLOW = 93,
    FG_BRIGHT_BLUE = 94,
    FG_BRIGHT_MAGENTA = 95,
    FG_BRIGHT_CYAN = 96,
    FG_BRIGHT_WHITE = 97,

    BG_BLACK = 40,
    BG_RED = 41,
    BG_GREEN = 42,
    BG_YELLOW = 43,
    BG_BLUE = 44,
    BG_MAGENTA = 45,
    BG_CYAN = 46,
    BG_WHITE = 47,

    BG_BRIGHT_BLACK = 100,
    BG_BRIGHT_RED = 101,
    BG_BRIGHT_GREEN = 102,
    BG_BRIGHT_YELLOW = 103,
    BG_BRIGHT_BLUE = 104,
    BG_BRIGHT_MAGENTA = 105,
    BG_BRIGHT_CYAN = 106,
    BG_BRIGHT_WHITE = 107,
};

inline std::string getAnsiCode(std::initializer_list<int> codes) {
    std::ostringstream oss;
    oss << "\033[";
    for (auto it = codes.begin(); it != codes.end(); ++it) {
        if (it != codes.begin()) oss << ";";
        oss << *it;
    }
    oss << "m";
    return oss.str();
}

inline std::string getAnsiCode(ANSI_Effect effect, ANSI_Color color) {
    return getAnsiCode({static_cast<int>(effect), static_cast<int>(color)});
}

inline std::string resetAnsi() {
    return "\033[0m";
}

#endif // ANSI_HPP