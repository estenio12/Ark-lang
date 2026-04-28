#include <iostream>
#include <fstream>
#include <filesystem>
#include "../src/Output.hpp"

int main(int argc, char** argv)
{
    if(argc < 3)
    {
        Ark::Output::PrintError("Expected file .ark and file .exp to run test.");
    }
    Ark::Output::Print("Hello");

    return EXIT_SUCCESS;
}