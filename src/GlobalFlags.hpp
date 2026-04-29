#pragma once
#include <string>

namespace Ark::Global
{
    namespace Flags
    {
        enum OUTPUT_FLAG { STDOUT, FILE, NONE };
        
        OUTPUT_FLAG PRINT_LEXER_OUTPUT = OUTPUT_FLAG::STDOUT;
        std::string PRINT_LEXER_OUTPUT_FILE = "";  
        
        OUTPUT_FLAG PRINT_PARSER_OUTPUT = OUTPUT_FLAG::STDOUT;
        std::string PRINT_PARSER_OUTPUT_FILE = ""; 
    }
}
