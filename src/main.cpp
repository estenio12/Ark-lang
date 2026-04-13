#include <iostream>
#include "Output.hpp"
#include "FileHandler.hpp"

int main(int argc, char** argv)
{
    Output::Initialize();

    if(argc <= 1)
    {
        Output::ThrowFatalError("Main file path is expected as an argument.");
        exit(1);
    }

    auto fh = new FileHandler(std::string(argv[1]));
    Output::Print(fh->GetContent());
    return EXIT_SUCCESS;
}