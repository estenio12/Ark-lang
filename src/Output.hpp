#pragma once

#include <iostream>
#include <string>

#ifdef _WIN32
#include <windows.h>
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
            DWORD dwMode = 0;
            GetConsoleMode(hOut, &dwMode);
            SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

            SetConsoleOutputCP(CP_UTF8);
            SetConsoleCP(CP_UTF8);
        #endif
    }

    inline void Print(std::string_view message, bool break_line = false, bool use_bold = false)
    {
        std::cout << (use_bold ? BOLD : "") << message << RESET << (break_line ? "\n" : "");
    }
    
    inline void PrintError(std::string_view message, bool break_line = false, bool use_bold = false)
    {
        std::cout << (use_bold ? BOLD : "") << RED << message << RESET << (break_line ? "\n" : "");
    }

    inline void PrintSuccess(std::string_view message, bool break_line = false, bool use_bold = false)
    {
        std::cout << (use_bold ? BOLD : "") << GREEN << message << RESET << (break_line ? "\n" : "");
    }

    inline void PrintWarn(std::string_view message, bool break_line = false, bool use_bold = false)
    {
        std::cout << (use_bold ? BOLD : "") << YELLOW << message << RESET << (break_line ? "\n" : "");
    }

    inline void PrintInfo(std::string_view message, bool break_line = false, bool use_bold = false)
    {
        std::cout << (use_bold ? BOLD : "") << BLUE << message << RESET << (break_line ? "\n" : "");
    }

    [[noreturn]] inline void ThrowFatalError(std::string_view message)
    {
        Output::PrintError(message, true, true);
        exit(1);
    }
};