#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <memory>
#include "FileHandler.hpp"
#include "TokenManager.hpp"

class Lexer
{
    private:
        std::string_view source;

    public:
        Lexer(std::shared_ptr<FileDescriptor> file_descriptor): file_descriptor(file_descriptor) 
        {
            source = file_descriptor->source_code;
        }
        std::unique_ptr<TokenManager> Tokenize();
    
    private:
        std::shared_ptr<FileDescriptor> file_descriptor;

    private:
        const char COMMENTARY = '#';
        const char WHITESPACE = ' ';
        const char TAB = '\t';
        const char RETURN = '\r';
        const char NEW_LINE = '\n';

    private:
        uint8_t IsDelimiter(int start_pos);

    private:
        std::string_view GetLexeme(size_t start, size_t end);
        void BuildToken(uint16_t start_pos, uint16_t end_pos, int64_t line, int64_t col);
};


