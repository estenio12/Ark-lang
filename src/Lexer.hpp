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
#include <cstdint>
#include <memory>
#include <unordered_set>
#include "FileHandler.hpp"
#include "TokenManager.hpp"

namespace Ark
{
    class Lexer
    {
        private:
            std::string_view source;
            std::unique_ptr<Ark::TokenManager> tokens;

        public:
            Lexer(const std::shared_ptr<Ark::FileDescriptor> file_descriptor): file_descriptor(file_descriptor) 
            {
                source = file_descriptor->source_code;
                tokens = std::make_unique<Ark::TokenManager>(file_descriptor);
            }
            std::unique_ptr<Ark::TokenManager> Tokenize();
        
        private:
            std::shared_ptr<Ark::FileDescriptor> file_descriptor;

        private:
            const char COMMENTARY = '#';
            const char WHITESPACE = ' ';
            const char TAB = '\t';
            const char RETURN = '\r';
            const char NEW_LINE = '\n';
            const char FORM_FEED = '\f';
            const char VERTICAL_TAB = '\v';

        private:
            bool IsNumber(const std::string& target, uint8_t max_dots);
            bool IsInteger(const std::string& target);
            bool IsFloat(const std::string& target);
            bool IsBoolean(const std::string& target);
            bool IsChar(const std::string& target);
            bool IsString(const std::string& target);
            bool IsKeyword(const std::string& target);
            bool IsOpArithmetic(const char& target);
            uint8_t IsDelimiter(const size_t& index);
            uint8_t IsOpLogic(const size_t& index);
            uint8_t IsOpComparison(const size_t& index);
            uint8_t IsOpBitwise(const size_t& index);
            uint8_t IsOpAssignment(const size_t& index);
            bool IsIdentifier(const std::string& target);

        private:
            void PopulateHashsets();
            Ark::TokenType FindType(const std::string& target);
            std::string GetLexeme(std::string& buffer);
            void BuildToken(std::string lexeme, uint64_t line, uint64_t col, Ark::TokenType type = Ark::TokenType::UNKNOWN);
    };
}

