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
            std::unordered_set<std::string_view> keywords;
            std::unordered_set<std::string_view> op_arithmetic;
            std::unordered_set<std::string_view> op_logic;
            std::unordered_set<std::string_view> op_comparison;
            std::unordered_set<std::string_view> op_bitwise;

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
            static bool IsDigit(const std::string_view& target, uint8_t max_dots);
            static bool IsInteger(const std::string_view& target);
            static bool IsFloat(const std::string_view& target);
            static bool IsBoolean(const std::string_view& target);
            static bool IsChar(const std::string_view& target);
            uint8_t IsDelimiter(size_t pos);
            bool IsKeyword(const std::string_view& target);
            bool IsOpArithmetic(const std::string_view& target);
            bool IsOpLogic(const std::string_view& target);
            bool IsOpComparison(const std::string_view& target);
            bool IsOpBitwise(const std::string_view& target);
            bool IsIdentifier(const std::string_view& target);

        private:
            void PopulateHashsets();
            Ark::TokenType FindType(const std::string_view& target);
            std::string_view GetLexeme(size_t start, size_t end);
            void BuildToken(uint16_t start_pos, uint16_t end_pos, uint64_t line, uint64_t col, Ark::TokenType type = Ark::TokenType::UNKNOWN);
    };
}

