#include <memory>
#include <chrono>
#include "Output.hpp"
#include "ModuleManeger.hpp"
#include "CoreCompile.hpp"

int main(int argc, char** argv)
{
    auto timer_start = std::chrono::high_resolution_clock::now();
    Ark::Output::Initialize();

    if(argc <= 1)
    {
        Ark::Output::ThrowFatalError("Main file path is expected as an argument.");
        exit(1);
    }

    auto module_manager = std::make_shared<Ark::ModuleResolver>();
    auto compile = std::make_unique<Ark::CoreCompile>(module_manager);
    compile->Compile(std::string(argv[1]));

    auto timer_end = std::chrono::high_resolution_clock::now();
    auto elipsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(timer_end - timer_start);

    Ark::Output::Print("\nCompilation completed in: " + std::to_string(elipsed_time.count()) + "ms\n");

    return EXIT_SUCCESS;
}