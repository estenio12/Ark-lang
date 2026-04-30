/*
 * Copyright 2026 Ark-lang Authors
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once
#include <vector>
#include <cstdint>
#include <memory>
#include <fstream>
#include "FileHandler.hpp"
#include "Output.hpp"
#include "GlobalFlags.hpp"

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
        OP_ASSIGNMENT,
        OP_COMPARISON,
        OP_BITWISE,
        DELIMITER,
        IDENTIFIER,
        END_OF_FILE, 
        UNKNOWN
    };

    struct Token
    {
        std::string content;
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
                if(Ark::Global::Flags::PRINT_LEXER_OUTPUT == Ark::Global::Flags::OUTPUT_FLAG::NONE) return;
                std::fstream file(Ark::Global::Flags::PRINT_LEXER_OUTPUT_FILE, std::ios::out);

                for(const auto& token : this->tokens)
                {
                    std::string outtoken;
                    outtoken += "\nContent: ";
                    outtoken += token.content;
                    outtoken += "\nType: ";
                    outtoken += TokenTypeString(token.type);
                    outtoken += "\nLine: ";
                    outtoken += std::to_string(token.line);
                    outtoken += "\nCol: ";
                    outtoken += std::to_string(token.col);
                    outtoken += "\n------------------------\n\n";

                    if(Ark::Global::Flags::PRINT_LEXER_OUTPUT == Ark::Global::Flags::OUTPUT_FLAG::STDOUT)
                    {
                        Ark::Output::Print(outtoken);
                    }
                    else
                    {
                        if(file.is_open() && file.good())
                        {
                            file << outtoken;
                        }
                    }
                }

                file.close();
            }

        private:
            std::string TokenTypeString(TokenType type)
            {
                if(type == TokenType::LITERAL_INT) return "LITERAL_INT";
                if(type == TokenType::LITERAL_FLOAT) return "LITERAL_FLOAT";
                if(type == TokenType::LITERAL_BOOL) return "LITERAL_BOOL";
                if(type == TokenType::LITERAL_CHAR) return "LITERAL_CHAR";
                if(type == TokenType::LITERAL_STRING) return "LITERAL_STRING";
                if(type == TokenType::KEYWORD) return "KEYWORD";
                if(type == TokenType::OP_ARITHMETIC) return "OP_ARITHMETIC";
                if(type == TokenType::OP_LOGICAL) return "OP_LOGICAL";
                if(type == TokenType::OP_ASSIGNMENT) return "OP_ASSIGNMENT";
                if(type == TokenType::OP_COMPARISON) return "OP_COMPARISON";
                if(type == TokenType::OP_BITWISE) return "OP_BITWISE";
                if(type == TokenType::DELIMITER) return "DELIMITER";
                if(type == TokenType::IDENTIFIER) return "IDENTIFIER";
                if(type == TokenType::END_OF_FILE) return "END_OF_FILE";
                return "UNKNOWN";
            }
    };
}