#include <iostream>
#include "Output.hpp"

int main(int argc, char** argv)
{
    Output::Initialize();
    Output::printError("Teste");
    Output::printSuccess("Teste");
    Output::printWarn("Teste");
    Output::printInfo("Teste");
    Output::print("Teste");
    return EXIT_SUCCESS;
}