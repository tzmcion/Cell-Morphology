#pragma once

namespace Colors {
    // Foreground (Text) Colors
    constexpr const char* RESET      = "\e[0m";    // Reset to default
    constexpr const char* BLACK      = "\e[30m";
    constexpr const char* RED        = "\e[31m";
    constexpr const char* GREEN      = "\e[32m";
    constexpr const char* YELLOW     = "\e[33m";
    constexpr const char* BLUE       = "\e[34m";
    constexpr const char* MAGENTA    = "\e[35m";
    constexpr const char* CYAN       = "\e[36m";
    constexpr const char* WHITE      = "\e[37m";

    // Bright Foreground (Text) Colors
    constexpr const char* BRIGHT_BLACK   = "\e[90m";
    constexpr const char* BRIGHT_RED     = "\e[91m";
    constexpr const char* BRIGHT_GREEN   = "\e[92m";
    constexpr const char* BRIGHT_YELLOW  = "\e[93m";
    constexpr const char* BRIGHT_BLUE    = "\e[94m";
    constexpr const char* BRIGHT_MAGENTA = "\e[95m";
    constexpr const char* BRIGHT_CYAN    = "\e[96m";
    constexpr const char* BRIGHT_WHITE   = "\e[97m";

    // Background Colors
    constexpr const char* BG_BLACK      = "\e[40m";
    constexpr const char* BG_RED        = "\e[41m";
    constexpr const char* BG_GREEN      = "\e[42m";
    constexpr const char* BG_YELLOW     = "\e[43m";
    constexpr const char* BG_BLUE       = "\e[44m";
    constexpr const char* BG_MAGENTA    = "\e[45m";
    constexpr const char* BG_CYAN       = "\e[46m";
    constexpr const char* BG_WHITE      = "\e[47m";

    // Bright Background Colors
    constexpr const char* BG_BRIGHT_BLACK   = "\e[100m";
    constexpr const char* BG_BRIGHT_RED     = "\e[101m";
    constexpr const char* BG_BRIGHT_GREEN   = "\e[102m";
    constexpr const char* BG_BRIGHT_YELLOW  = "\e[103m";
    constexpr const char* BG_BRIGHT_BLUE    = "\e[104m";
    constexpr const char* BG_BRIGHT_MAGENTA = "\e[105m";
    constexpr const char* BG_BRIGHT_CYAN    = "\e[106m";
    constexpr const char* BG_BRIGHT_WHITE   = "\e[107m";
};