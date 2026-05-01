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
    void BuildArguments(int argc, char** args)
    {
        if(argc < 2) return;
        
        for(int i = 1; i < argc; i++)
        {
            auto cmd = std::string(args[i]);

            // # Mark the output compilation path.
            if(cmd == "--o")
            {
                if(!((i + 1) < argc)) Ark::Output::ThrowFatalError("The output file path not passed");
                Ark::Global::Flags::OUTPUT_COMPILATION_FILE = std::string(args[i + 1]);
                i++;
            }

            // # Mark to print lexer result into output.
            if(cmd == "--plex")
            {
                if(!((i + 1) < argc)) Ark::Output::ThrowFatalError("The plex output file path not passed");
                Ark::Global::Flags::PRINT_LEXER_OUTPUT = Ark::Global::Flags::OUTPUT_FLAG::FILE;
                Ark::Global::Flags::PRINT_LEXER_OUTPUT_FILE = std::string(args[i + 1]);
                i++;
            }
        }
    }
}

