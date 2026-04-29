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
            Ark::Output::Print(args[i]);
        }
    }
}

