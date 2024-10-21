#pragma once

namespace Colors {
    // Foreground (Text) Colors
    const char* RESET      = "\e[0m";    // Reset to default
    const char* BLACK      = "\e[30m";
    const char* RED        = "\e[31m";
    const char* GREEN      = "\e[32m";
    const char* YELLOW     = "\e[33m";
    const char* BLUE       = "\e[34m";
    const char* MAGENTA    = "\e[35m";
    const char* CYAN       = "\e[36m";
    const char* WHITE      = "\e[37m";

    // Bright Foreground (Text) Colors
    const char* BRIGHT_BLACK   = "\e[90m";
    const char* BRIGHT_RED     = "\e[91m";
    const char* BRIGHT_GREEN   = "\e[92m";
    const char* BRIGHT_YELLOW  = "\e[93m";
    const char* BRIGHT_BLUE    = "\e[94m";
    const char* BRIGHT_MAGENTA = "\e[95m";
    const char* BRIGHT_CYAN    = "\e[96m";
    const char* BRIGHT_WHITE   = "\e[97m";

    // Background Colors
    const char* BG_BLACK      = "\e[40m";
    const char* BG_RED        = "\e[41m";
    const char* BG_GREEN      = "\e[42m";
    const char* BG_YELLOW     = "\e[43m";
    const char* BG_BLUE       = "\e[44m";
    const char* BG_MAGENTA    = "\e[45m";
    const char* BG_CYAN       = "\e[46m";
    const char* BG_WHITE      = "\e[47m";

    // Bright Background Colors
    const char* BG_BRIGHT_BLACK   = "\e[100m";
    const char* BG_BRIGHT_RED     = "\e[101m";
    const char* BG_BRIGHT_GREEN   = "\e[102m";
    const char* BG_BRIGHT_YELLOW  = "\e[103m";
    const char* BG_BRIGHT_BLUE    = "\e[104m";
    const char* BG_BRIGHT_MAGENTA = "\e[105m";
    const char* BG_BRIGHT_CYAN    = "\e[106m";
    const char* BG_BRIGHT_WHITE   = "\e[107m";
}