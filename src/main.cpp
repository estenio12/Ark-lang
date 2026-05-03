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
#include <memory>
#include <chrono>
#include "Output.hpp"
#include "ModuleManeger.hpp"
#include "CoreCompile.hpp"
#include "CompilerArguments.hpp"

int main(int argc, char** argv)
{
    auto timer_start = std::chrono::high_resolution_clock::now();
    
    Ark::Output::Initialize();
    Ark::CompilerArguments::BuildArguments(argc, argv);

    if(Ark::Global::Flags::SOURCE_FILE.empty())
    {
        Ark::Output::PrintError("No source file provided. Use --h for help.");
        return EXIT_FAILURE; 
    }

    try 
    {
        auto module_manager = std::make_shared<Ark::ModuleResolver>();
        auto compile = std::make_unique<Ark::CoreCompile>(module_manager);
        
        compile->Compile(Ark::Global::Flags::SOURCE_FILE);
        
        auto timer_end = std::chrono::high_resolution_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(timer_end - timer_start);
        
        if(Ark::Global::Flags::PRINT_TIMER)
            Ark::Output::Print("\nCompilation completed in: " + std::to_string(elapsed_time.count()) + "ms\n");
    }
    catch (const std::exception& e) 
    {
        Ark::Output::ThrowFatalError("Core", std::string("Unexpected error: ") + e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}