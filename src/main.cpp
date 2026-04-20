#include <iostream>
#include <memory>
#include <chrono>
#include "Output.hpp"
#include "ModuleManeger.hpp"
#include "CoreCompile.hpp"

int main(int argc, char** argv)
{
    auto timer_start = std::chrono::high_resolution_clock::now();
    Output::Initialize();

    if(argc <= 1)
    {
        Output::ThrowFatalError("Main file path is expected as an argument.");
        exit(1);
    }

    auto module_manager = std::make_shared<ModuleManager::Manager>();
    auto compile = std::make_unique<CoreCompile>(module_manager);
    compile->Compile(std::string(argv[1]));

    auto timer_end = std::chrono::high_resolution_clock::now();
    auto elipsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(timer_end - timer_start);

    Output::Print("\nCompilation completed in: " + std::to_string(elipsed_time.count()) + "ms");

    return EXIT_SUCCESS;
}