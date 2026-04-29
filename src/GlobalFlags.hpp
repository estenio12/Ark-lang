#pragma once
#include <string>
#include <cstdint>

namespace Ark::Global
{
    namespace Flags
    {
        enum class OUTPUT_FLAG : uint8_t { STDOUT, FILE, NONE };
        
        inline OUTPUT_FLAG PRINT_LEXER_OUTPUT = OUTPUT_FLAG::NONE;
        inline std::string PRINT_LEXER_OUTPUT_FILE = "";  
        
        inline OUTPUT_FLAG PRINT_PARSER_OUTPUT = OUTPUT_FLAG::STDOUT;
        inline std::string PRINT_PARSER_OUTPUT_FILE = ""; 
    }
}
