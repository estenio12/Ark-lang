#pragma once
#include <vector>
#include <cstdint>
#include <memory>
#include "FileHandler.hpp"

namespace Ark
{
    enum class TokenType : uint8_t
    {
        LITERAL_INT,
        LITERAL_FLOAT,
        LITERAL_BOOL,
        LITERAL_CHAR,
        LITERAL_STRING,
        KEYWORD,
        OP_ARITHMETIC,
        OP_LOGICAL,
        OP_COMPARISON,
        OP_BITWISE,
        DELIMITER,
        IDENTIFIER,
        END_OF_FILE, 
        UNKNOWN
    };


    struct Token
    {
        std::string_view content;
        TokenType type;
        uint32_t line;
        uint32_t col;
    };

    class TokenManager
    {
        private:
            std::shared_ptr<FileDescriptor> file_descriptor;
            std::vector<Token> tokens;
            size_t cursor = 0;

        public:
            TokenManager(std::shared_ptr<FileDescriptor> file_descriptor) : file_descriptor(file_descriptor) 
            {
                tokens.reserve(file_descriptor->file_size / 5);
            }

        public:
            void PushToken(Token token){ this->tokens.push_back(token); }
            bool IsAtEnd() const { return cursor >= tokens.size(); }
        
            const Token& Peek(size_t offset = 0) const 
            {
                if (cursor + offset >= tokens.size()) return tokens.back();
                return tokens[cursor + offset];
            }
        
            const Token& Consume() 
            {
                if (cursor < tokens.size()) return tokens[cursor++];
                return tokens.back();
            }
    };
}