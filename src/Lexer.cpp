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
#include <string>
#include "Lexer.hpp"
#include "Definitions.hpp"
#include "Output.hpp"

std::unique_ptr<Ark::TokenManager> Ark::Lexer::Tokenize()
{
    int64_t line = 1;
    int64_t col  = 0;
    std::string buffer;
    buffer.reserve(256);

    size_t source_size = file_descriptor->source_code.size();
    for(size_t i = 0; i < source_size; i++)
    {
        char letter = source[i];
        
        if ((static_cast<unsigned char>(letter) & 0xC0) != 0x80) col++;
        
        if(letter == TAB || letter == RETURN) continue;

        if(letter == NEW_LINE)
        {
            line++;
            col = 0;
            continue;
        }

        if(letter == COMMENTARY)
        {
            while (i < source_size && source[i] != NEW_LINE) 
            { 
                i++; 
                col = 0;
            }
            line++;
            continue;
        }

        if(letter == WHITESPACE)
        {
            this->BuildToken(this->GetLexeme(buffer), line, col);
            continue;
        }

        if(letter == Ark::DELIMITER::QUOTE[0])
        {
            this->BuildToken(this->GetLexeme(buffer), line, col - 1);

            std::string slice {letter};
            bool is_escaped = false;
            while((i + 1) < source_size)
            {
                i++; col++;
                char current = source[i];
                slice.push_back(current);

                if (current == Ark::DELIMITER::QUOTE[0] && !is_escaped) break;

                if (current == '\\')
                    is_escaped = !is_escaped;
                else
                    is_escaped = false;
            }
            this->BuildToken(this-> GetLexeme(slice), line, col);
            continue;
        }

        if(letter == Ark::DELIMITER::DOUBLEQUOTE[0])
        {
            this->BuildToken(this->GetLexeme(buffer), line, col -1 );

            std::string slice {letter};
            bool is_escaped = false;
            while((i + 1) < source_size)
            {
                i++; col++;
                char current = source[i];
                slice.push_back(current);

                if (current == Ark::DELIMITER::DOUBLEQUOTE[0] && !is_escaped) break;

                if (current == '\\')
                    is_escaped = !is_escaped;
                else
                    is_escaped = false;
            }
            this->BuildToken(this-> GetLexeme(slice), line, col);
            continue;
        }

        auto delimiter = IsDelimiter(i);
        if(delimiter > 0)
        {
            this->BuildToken(this->GetLexeme(buffer), line, col);
            
            if(delimiter == 1)
            {
                this->BuildToken(std::string {letter}, line, col + 1, Ark::TokenType::DELIMITER);
            }
            else
            {
                this->BuildToken(std::string { letter, this->source[i + 1] }, line, col + 1, Ark::TokenType::DELIMITER);
                i++;
            }

            continue;
        }

        if(IsOpArithmetic(letter))
        {
            this->BuildToken(this->GetLexeme(buffer), line, col - 1);
            this->BuildToken(std::string{letter}, line, col, Ark::TokenType::OP_ARITHMETIC);
            continue;
        }

        auto logical = IsOpLogic(i);
        if(logical > 0)
        {
            this->BuildToken(this->GetLexeme(buffer), line, col - 1);
            
            if(logical == 1)
            {
                this->BuildToken(std::string {letter}, line, col + 1, Ark::TokenType::OP_LOGICAL);
            }
            else
            {
                this->BuildToken(std::string { letter, this->source[i + 1] }, line, col + 1, Ark::TokenType::OP_LOGICAL);
                i++;
            }

            continue;
        }

        auto assignment = IsOpAssignment(i);
        if(assignment > 0)
        {
            this->BuildToken(this->GetLexeme(buffer), line, col - 1);
            
            if(assignment == 1)
            {
                this->BuildToken(std::string {letter}, line, col + 1, Ark::TokenType::OP_ASSIGNMENT);
            }
            else
            {
                this->BuildToken(std::string { letter, this->source[i + 1] }, line, col + 1, Ark::TokenType::OP_ASSIGNMENT);
                i++;
            }

            continue;
        }

        auto comparison = IsOpComparison(i);
        if(comparison > 0)
        {
            this->BuildToken(this->GetLexeme(buffer), line, col - 1);
            
            if(comparison == 1)
            {
                this->BuildToken(std::string {letter}, line, col + 1, Ark::TokenType::OP_COMPARISON);
            }
            else
            {
                this->BuildToken(std::string { letter, this->source[i + 1] }, line, col + 1, Ark::TokenType::OP_COMPARISON);
                i++;
            }

            continue;
        }

        auto bitwise = IsOpBitwise(i);
        if(bitwise > 0)
        {
            this->BuildToken(this->GetLexeme(buffer), line, col - 1);
            
            if(bitwise == 1)
            {
                this->BuildToken(std::string {letter}, line, col + 1, Ark::TokenType::OP_BITWISE);
            }
            else
            {
                this->BuildToken(std::string { letter, this->source[i + 1] }, line, col + 1, Ark::TokenType::OP_BITWISE);
                i++;
            }

            continue;
        }

        buffer.push_back(letter);
    }

    if(buffer.size() > 0)
        this->BuildToken(this->GetLexeme(buffer), line, col);

    this->tokens->printTokens();
    return std::move(this->tokens);
}

void Ark::Lexer::BuildToken(std::string lexeme, uint64_t line, uint64_t col, Ark::TokenType type)
{
    if(lexeme.empty()) return;

    Ark::Token token;
    token.col = col - lexeme.size();
    token.line = line;
    token.content = lexeme;

    if(type == Ark::TokenType::UNKNOWN)
        token.type = this->FindType(token.content);
    else
        token.type = type;

    this->tokens->PushToken(token);
}

std::string Ark::Lexer::GetLexeme(std::string& buffer)
{
    auto lexeme = buffer;
    buffer.clear();
    return lexeme;
}

Ark::TokenType Ark::Lexer::FindType(const std::string& target)
{
    if(Ark::Lexer::IsInteger(target)) return Ark::TokenType::LITERAL_INT;
    if(Ark::Lexer::IsFloat(target)) return Ark::TokenType::LITERAL_FLOAT;
    if(Ark::Lexer::IsBoolean(target)) return Ark::TokenType::LITERAL_BOOL;
    if(Ark::Lexer::IsChar(target)) return Ark::TokenType::LITERAL_CHAR;
    if(Ark::Lexer::IsKeyword(target)) return Ark::TokenType::KEYWORD;
    if(Ark::Lexer::IsString(target)) return Ark::TokenType::LITERAL_STRING;
    if(Ark::Lexer::IsIdentifier(target)) return Ark::TokenType::IDENTIFIER;

    return Ark::TokenType::UNKNOWN;
}

bool Ark::Lexer::IsDigit(const std::string& target, uint8_t max_dots)
{
    if (target.empty()) return false;

    uint8_t dots_found = 0;

    for(const auto& letter : target)
    {
        if(std::isdigit(static_cast<unsigned char>(letter)) == 0)
        {
            if(letter == '.')
            {
                dots_found++;
                if(dots_found > max_dots) return false;
            }
            else
            {
                return false;
            }
        }
    }

    return max_dots == 0 ? true : (dots_found == max_dots);
}

bool Ark::Lexer::IsInteger(const std::string& target)
{
    return Ark::Lexer::IsDigit(target, 0);
}

bool Ark::Lexer::IsFloat(const std::string& target)
{
    if(!Ark::Lexer::IsDigit(target, 1)) return false;
    if(target.front() == '.' || target.back() == '.') return false;
    return true;
}

bool Ark::Lexer::IsBoolean(const std::string& target)
{
    return target == Ark::KEYWORDS::TTRUE || target == Ark::KEYWORDS::TFALSE;
}

bool Ark::Lexer::IsChar(const std::string& target)
{
    if(target.size() < 3) return false;
    if(target.front() != '\'' || target.back() != '\'') return false;

    return true;
}

bool Ark::Lexer::IsKeyword(const std::string& target)
{
    return Ark::KEYWORDS::keywords.find(target) != Ark::KEYWORDS::keywords.end();
}

bool Ark::Lexer::IsOpArithmetic(const char& target)
{
    if(target == Ark::OP_ARITHMETIC::ADD[0] ||
       target == Ark::OP_ARITHMETIC::SUB[0] ||
       target == Ark::OP_ARITHMETIC::MUL[0] ||
       target == Ark::OP_ARITHMETIC::DIV[0] )
    {
        return true;
    }

    return false;
}

uint8_t Ark::Lexer::IsOpLogic(const size_t& index)
{
    char letter = source[index];
    char next = (index + 1 < this->source.size()) ? source[index + 1] : '\0';

    if(next != '\0')
    {
        if(Ark::OP_LOGICAL::OR[0] == letter && Ark::OP_LOGICAL::OR[1] == next) return 2;
        if(Ark::OP_LOGICAL::AND[0] == letter && Ark::OP_LOGICAL::AND[1] == next) return 2;
    }

    if(letter == Ark::OP_LOGICAL::NOT[0] && next != Ark::OP_ASSIGNMENT::ASSIGN[0]) return 1;
    return 0;
}

uint8_t Ark::Lexer::IsOpComparison(const size_t& index)
{
    char letter = source[index];
    char next = (index + 1 < this->source.size()) ? source[index + 1] : '\0';

    if(next != '\0')
    {
        if(Ark::OP_COMPARISON::EQ[0]   == letter && Ark::OP_COMPARISON::EQ[1]   == next) return 2;
        if(Ark::OP_COMPARISON::DIFF[0] == letter && Ark::OP_COMPARISON::DIFF[1] == next) return 2;
        if(Ark::OP_COMPARISON::GTEQ[0] == letter && Ark::OP_COMPARISON::GTEQ[1] == next) return 2;
        if(Ark::OP_COMPARISON::LTEQ[0] == letter && Ark::OP_COMPARISON::LTEQ[1] == next) return 2;
    }

    if(letter == Ark::OP_COMPARISON::GT[0]) return 1;
    if(letter == Ark::OP_COMPARISON::LT[0]) return 1;
    return 0;
}

uint8_t Ark::Lexer::IsOpBitwise(const size_t& index)
{
    char letter = source[index];
    char next = (index + 1 < this->source.size()) ? source[index + 1] : '\0';

    if(next != '\0')
    {
        if(Ark::OP_BITWISE::LSHIFT[0] == letter && Ark::OP_BITWISE::LSHIFT[1] == next) return 2;
        if(Ark::OP_BITWISE::RSHIFT[0] == letter && Ark::OP_BITWISE::RSHIFT[1] == next) return 2;
    }

    if(letter == Ark::OP_BITWISE::OR[0])  return 1;
    if(letter == Ark::OP_BITWISE::AND[0]) return 1;
    if(letter == Ark::OP_BITWISE::XOR[0]) return 1;
    if(letter == Ark::OP_BITWISE::NOT[0]) return 1;
    return 0;
}

uint8_t Ark::Lexer::IsDelimiter(const size_t& index)
{
    char letter = source[index];
    char next = (index + 1 < this->source.size()) ? source[index + 1] : '\0';

    if(next != '\0')
    {
        if(Ark::DELIMITER::ARROW[0] == letter && Ark::DELIMITER::ARROW[1] == next) return 2;
        if(Ark::DELIMITER::SCOPEACCESS[0] == letter && Ark::DELIMITER::SCOPEACCESS[1] == next) return 2;
    }

    if(letter == Ark::DELIMITER::COMMA[0]) return 1;
    if(letter == Ark::DELIMITER::COLON[0]) return 1;
    if(letter == Ark::DELIMITER::LPARAN[0]) return 1;
    if(letter == Ark::DELIMITER::RPARAN[0]) return 1;
    if(letter == Ark::DELIMITER::LBRACE[0]) return 1;
    if(letter == Ark::DELIMITER::RBRACE[0]) return 1;
    if(letter == Ark::DELIMITER::LBRACKET[0]) return 1;
    if(letter == Ark::DELIMITER::RBRACKET[0]) return 1;
    if(letter == Ark::DELIMITER::SEMICOLON[0]) return 1;
    if(letter == Ark::DELIMITER::QUOTE[0]) return 1;
    if(letter == Ark::DELIMITER::DOUBLEQUOTE[0]) return 1;

    return 0;
}

uint8_t Ark::Lexer::IsOpAssignment(const size_t& index)
{
    char letter = source[index];
    char next = (index + 1 < this->source.size()) ? source[index + 1] : '\0';

    if(next != '\0')
    {
        if(Ark::OP_ASSIGNMENT::ADDASSIGN[0] == letter && Ark::OP_ASSIGNMENT::ADDASSIGN[1] == next) return 2;
        if(Ark::OP_ASSIGNMENT::SUBASSIGN[0] == letter && Ark::OP_ASSIGNMENT::SUBASSIGN[1] == next) return 2;
        if(Ark::OP_ASSIGNMENT::DIVASSIGN[0] == letter && Ark::OP_ASSIGNMENT::DIVASSIGN[1] == next) return 2;
        if(Ark::OP_ASSIGNMENT::MULTASSIGN[0] == letter && Ark::OP_ASSIGNMENT::MULTASSIGN[1] == next) return 2;
    }

    if(letter == Ark::OP_ASSIGNMENT::ASSIGN[0] && next != Ark::OP_ASSIGNMENT::ASSIGN[0]) return 1;

    return 0;
}

bool Ark::Lexer::IsIdentifier(const std::string& target)
{
    if (target.empty()) return false;

    char first = target.front();
    bool isValidFirst = (first >= 'a' && first <= 'z') || 
                        (first >= 'A' && first <= 'Z') || 
                        (first == '_');
    
    if (!isValidFirst) return false;

    for (size_t i = 1; i < target.size(); ++i)
    {
        char c = target[i];
        bool isValidChar = (c >= 'a' && c <= 'z') || 
                           (c >= 'A' && c <= 'Z') || 
                           (c >= '0' && c <= '9') || 
                           (c == '_');
        
        if (!isValidChar) return false;
    }

    return true;
}

bool Ark::Lexer::IsString(const std::string& target)
{
    if(target.size() < 2) return false;
    char quote = Ark::DELIMITER::DOUBLEQUOTE[0];
    if(target.front() == quote && target.back() == quote) return true;
    return false;
}
