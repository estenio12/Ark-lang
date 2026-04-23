#pragma once
#include <vector>
#include <cstdint>
#include <memory>
#include "FileHandler.hpp"
#include "Output.hpp"

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

            void printTokens()
            {
                for(const auto& token : this->tokens)
                {
                    Output::Print("Content: ");
                    Output::Print(token.content);
                    Output::Print("Type: ");
                    Output::Print(TokenTypeString(token.type));
                    Output::Print("Line: ");
                    Output::Print(std::to_string(token.line));
                    Output::Print("Col: ");
                    Output::Print(std::to_string(token.col));
                    Output::Print("------------------------\n\n");
                }
            }

        private:
            std::string TokenTypeString(TokenType type)
            {
                if(type == TokenType::LITERAL_INT) return "LITERAL_INT";
                if(type == TokenType::LITERAL_INT) return "LITERAL_FLOAT";
                if(type == TokenType::LITERAL_INT) return "LITERAL_BOOL";
                if(type == TokenType::LITERAL_INT) return "LITERAL_CHAR";
                if(type == TokenType::LITERAL_INT) return "LITERAL_STRING";
                if(type == TokenType::LITERAL_INT) return "KEYWORD";
                if(type == TokenType::LITERAL_INT) return "OP_ARITHMETIC";
                if(type == TokenType::LITERAL_INT) return "OP_LOGICAL";
                if(type == TokenType::LITERAL_INT) return "OP_COMPARISON";
                if(type == TokenType::LITERAL_INT) return "OP_BITWISE";
                if(type == TokenType::LITERAL_INT) return "DELIMITER";
                if(type == TokenType::LITERAL_INT) return "IDENTIFIER";
                if(type == TokenType::LITERAL_INT) return "END_OF_FILE";
                return "UNKNOWN";
            }
    };
}