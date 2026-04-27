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

        private:
            bool IsDigit(const std::string& target, uint8_t max_dots);
            bool IsInteger(const std::string& target);
            bool IsFloat(const std::string& target);
            bool IsBoolean(const std::string& target);
            bool IsChar(const std::string& target);
            bool IsString(const std::string& target);
            uint8_t IsDelimiter(const size_t& index);
            bool IsKeyword(const std::string& target);
            bool IsOpArithmetic(const char& target);
            uint8_t IsOpLogic(const size_t& index);
            bool IsOpComparison(const std::string& target);
            bool IsOpBitwise(const std::string& target);
            bool IsIdentifier(const std::string& target);

        private:
            void PopulateHashsets();
            Ark::TokenType FindType(const std::string& target);
            std::string GetLexeme(std::string& buffer);
            void BuildToken(std::string lexeme, uint64_t line, uint64_t col, Ark::TokenType type = Ark::TokenType::UNKNOWN);
    };
}

