#include "Lexer.hpp"
#include "Definitions.hpp"
#include "Output.hpp"

std::unique_ptr<TokenManager> Lexer::Tokenize()
{
    int64_t line = 1;
    int64_t col  = 0;
    uint16_t start_rect = 0;

    char letter;
    size_t source_size = file_descriptor->source_code.size();
    for(size_t i = 0; i < source_size; i++)
    {
        col++;
        letter = source[i];
        
        if(letter == TAB) continue;

        if(letter == NEW_LINE)
        {
            line++;
            col = 0;
            continue;
        }

        if(letter == COMMENTARY)
        {
            while (i < source_size && source[i] != NEW_LINE) { i++; }
            start_rect = ++i;
            continue;
        }

        if(letter == WHITESPACE)
        {
            this->BuildToken(start_rect, i, line, col);
            start_rect = i + 1;
            continue;
        }

        auto delimiter = IsDelimiter(i);
        if(delimiter > 0)
        {
            // # Build token with first rect.
            this->BuildToken(start_rect, i, line, col, TokenType::DELIMITER);

            // # Build token with the delimiter founded.
            this->BuildToken(i, i + delimiter, line, col, TokenType::DELIMITER);
            start_rect = i + 1;
            continue;
        }
    }

    return std::move(this->tokens);
}

uint8_t Lexer::IsDelimiter(size_t pos)
{
    auto safe_range = (pos + 1) < source.size();
    if(safe_range && source.substr(pos, 2) == DELIMITER::ARROW) return 2;
    if(safe_range && source.substr(pos, 2) == DELIMITER::SCOPEACCESS) return 2;
    if(source[pos] == DELIMITER::COMMA[0]) return 1;
    if(source[pos] == DELIMITER::COLON[0]) return 1;
    if(source[pos] == DELIMITER::LPARAN[0]) return 1;
    if(source[pos] == DELIMITER::RPARAN[0]) return 1;
    if(source[pos] == DELIMITER::LBRACE[0]) return 1;
    if(source[pos] == DELIMITER::RBRACE[0]) return 1;
    if(source[pos] == DELIMITER::LBRACKET[0]) return 1;
    if(source[pos] == DELIMITER::RBRACKET[0]) return 1;
    if(source[pos] == DELIMITER::SEMICOLON[0]) return 1;

    return 0;
}

void Lexer::BuildToken(uint16_t start_pos, uint16_t end_pos, uint64_t line, uint64_t col, TokenType type)
{
    auto lexeme = this->GetLexeme(start_pos, end_pos);

    if(lexeme[0] == this->WHITESPACE) return;

    Token token;
    token.col = col;
    token.line = line;
    token.content = lexeme;
    
    Output::Print(token.content);
    Output::Print("\n");

    if(type == TokenType::UNKNOWN)
        token.type = this->FindType(token.content);
    else
        token.type = type;

    this->tokens->PushToken(token);
}

std::string_view Lexer::GetLexeme(size_t start, size_t end)
{
    return this->source.substr(start, end - start);
}

TokenType Lexer::FindType(const std::string_view& target)
{
    if(Lexer::IsInteger(target)) return TokenType::LITERAL_INT;
    if(Lexer::IsFloat(target)) return TokenType::LITERAL_FLOAT;
    if(Lexer::IsBoolean(target)) return TokenType::LITERAL_BOOL;

    return TokenType::UNKNOWN;
}

bool Lexer::IsDigit(const std::string_view& target, uint8_t max_dots)
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

bool Lexer::IsInteger(const std::string_view& target)
{
    return Lexer::IsDigit(target, 0);
}

bool Lexer::IsFloat(const std::string_view& target)
{
    if(!Lexer::IsDigit(target, 1)) return false;
    if(target.front() == '.' || target.back() == '.') return false;
    return true;
}

bool IsBoolean(const std::string_view& target)
{
    return target == KEYWORDS::TTRUE || target == KEYWORDS::TFALSE;
}

