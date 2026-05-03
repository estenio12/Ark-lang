/*
 * Copyright 2026 Ark-lang Authors
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once
#include <string>
#include <vector>
#include "GlobalFlags.hpp"
#include "Output.hpp"

namespace Ark::CompilerArguments
{
    void PrintVersion();
    void PrintHelp();

    void BuildArguments(int argc, char** args)
    {
        if(argc < 2) return;
        
        for(int i = 1; i < argc; i++)
        {
            std::string_view cmd = args[i];

            if(cmd == "--f")
            {
                if(i + 1 >= argc) Ark::Output::ThrowFatalError("CompilerArguments", "The source file path not passed");
                Ark::Global::Flags::SOURCE_FILE = args[++i];
            }
            else if(cmd == "--o" || cmd == "-o" || cmd == "--out" || cmd == "--output")
            {
                if(i + 1 >= argc) Ark::Output::ThrowFatalError("CompilerArguments", "The output file path not passed");
                Ark::Global::Flags::OUTPUT_COMPILATION_FILE = args[++i];
            }
            else if(cmd == "--plex")
            {
                if(i + 1 >= argc) Ark::Output::ThrowFatalError("CompilerArguments", "The plex output file path not passed");
                Ark::Global::Flags::PRINT_LEXER_OUTPUT = Ark::Global::Flags::OUTPUT_FLAG::FILE;
                Ark::Global::Flags::PRINT_LEXER_OUTPUT_FILE = args[++i];
            }
            else if(cmd == "--v" || cmd == "--version" || cmd == "-v")
            {
                PrintVersion();
                exit(0);
            }
            else if(cmd == "--h" || cmd == "--help" || cmd == "-h")
            {
                PrintHelp();
            }
            else if(cmd == "--no-timer")
            {
                Ark::Global::Flags::PRINT_TIMER = false;
            }
            else 
            {
                if (cmd.length() > 0 && cmd[0] != '-') 
                {
                    if (Ark::Global::Flags::SOURCE_FILE.empty()) 
                        Ark::Global::Flags::SOURCE_FILE = std::string(cmd);
                    else 
                        Ark::Output::ThrowFatalError("CompilerArguments", "Multiple source files detected or invalid argument: " + std::string(cmd));
                }
                else 
                {
                    Ark::Output::ThrowFatalError("CompilerArguments", "Unknown flag: " + std::string(cmd));
                }
            }
        }
    }

    void PrintVersion()
    {
        Ark::Output::PrintInfo("ID: ", false);
        Ark::Output::Print(Ark::Global::Flags::BUILD_ID);
        Ark::Output::PrintInfo("Version: ", false);
        Ark::Output::Print(Ark::Global::Flags::BUILD_VERSION);
        Ark::Output::PrintInfo("Date: ", false);
        Ark::Output::Print(Ark::Global::Flags::BUILD_DATE);
        Ark::Output::PrintInfo("License: ", false);
        Ark::Output::Print(Ark::Global::Flags::BUILD_LICENSE);
    }

    void PrintHelp()
    {
        PrintVersion();
        Ark::Output::Print("\nUsage: arkc [file] [options]\n");
        Ark::Output::Print("Options:");
        Ark::Output::Print("  --f <path>      Specify source file");
        Ark::Output::Print("  --o <path>      Specify output binary path");
        Ark::Output::Print("  --plex <path>   Print lexer result into output file");
        Ark::Output::Print("  --v, --version  Show version information");
        Ark::Output::Print("  --h, --help     Show this help message");
        Ark::Output::Print("  --no-timer      Disable compilation timer");
    }
}

