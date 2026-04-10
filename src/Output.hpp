#pragma once

#include <iostream>
#include <string>

#ifdef _WIN32
    #include <windows.h>
    #ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
        #define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
    #endif
#endif


namespace Output
{
    constexpr std::string_view RESET   = "\033[0m";
    constexpr std::string_view RED     = "\033[31m";
    constexpr std::string_view GREEN   = "\033[32m";
    constexpr std::string_view YELLOW  = "\033[33m";
    constexpr std::string_view BOLD    = "\033[1m";
    constexpr std::string_view BLUE    = "\033[34m";

    inline void Initialize() 
    {
        #ifdef _WIN32
            HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
            if (hOut == INVALID_HANDLE_VALUE) return;
            DWORD dwMode = 0;
            if (GetConsoleMode(hOut, &dwMode)) {
                SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
            }
        #endif
    }

    inline void print(std::string_view message, bool break_line = false, bool use_bold = false)
    {
        std::cout << (use_bold ? BOLD : "") << message << RESET << (break_line ? "\n" : "");
    }
    
    inline void printError(std::string_view message, bool break_line = false, bool use_bold = false)
    {
        std::cout << (use_bold ? BOLD : "") << RED << message << RESET << (break_line ? "\n" : "");
    }

    inline void printSuccess(std::string_view message, bool break_line = false, bool use_bold = false)
    {
        std::cout << (use_bold ? BOLD : "") << GREEN << message << RESET << (break_line ? "\n" : "");
    }

    inline void printWarn(std::string_view message, bool break_line = false, bool use_bold = false)
    {
        std::cout << (use_bold ? BOLD : "") << YELLOW << message << RESET << (break_line ? "\n" : "");
    }

    inline void printInfo(std::string_view message, bool break_line = false, bool use_bold = false)
    {
        std::cout << (use_bold ? BOLD : "") << BLUE << message << RESET << (break_line ? "\n" : "");
    }
};