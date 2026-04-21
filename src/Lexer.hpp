#pragma once
#include <cstdint>
#include <memory>
#include "FileHandler.hpp"
#include "TokenManager.hpp"

class Lexer
{
    private:
        std::string_view source;
        std::unique_ptr<TokenManager> tokens;

    public:
        Lexer(const std::shared_ptr<FileDescriptor> file_descriptor): file_descriptor(file_descriptor) 
        {
            source = file_descriptor->source_code;
            tokens = std::make_unique<TokenManager>(file_descriptor);
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
        uint8_t IsDelimiter(size_t pos);
        static bool IsDigit(const std::string_view& target, uint8_t max_dots);
        // bool IsAlphaDigit(const std::string_view& target);
        static bool IsInteger(const std::string_view& target);
        static bool IsFloat(const std::string_view& target);
        static bool IsBoolean(const std::string_view& target);

    private:
        TokenType FindType(const std::string_view& target);
        std::string_view GetLexeme(size_t start, size_t end);
        void BuildToken(uint16_t start_pos, uint16_t end_pos, uint64_t line, uint64_t col, TokenType type = TokenType::UNKNOWN);
};


