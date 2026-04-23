#include "Lexer.hpp"
#include "Definitions.hpp"
#include "Output.hpp"

std::unique_ptr<Ark::TokenManager> Ark::Lexer::Tokenize()
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
            this->BuildToken(start_rect, i, line, col, Ark::TokenType::DELIMITER);

            // # Build token with the delimiter founded.
            this->BuildToken(i, i + delimiter, line, col, Ark::TokenType::DELIMITER);
            start_rect = i + 1;
            continue;
        }
    }

    return std::move(this->tokens);
}

void Ark::Lexer::BuildToken(uint16_t start_pos, uint16_t end_pos, uint64_t line, uint64_t col, Ark::TokenType type)
{
    auto lexeme = this->GetLexeme(start_pos, end_pos);

    if(lexeme[0] == this->WHITESPACE) return;

    Ark::Token token;
    token.col = col;
    token.line = line;
    token.content = lexeme;
    
    Ark::Output::Print(token.content);
    Ark::Output::Print("\n");

    if(type == Ark::TokenType::UNKNOWN)
        token.type = this->FindType(token.content);
    else
        token.type = type;

    this->tokens->PushToken(token);
}

std::string_view Ark::Lexer::GetLexeme(size_t start, size_t end)
{
    return this->source.substr(start, end - start);
}

Ark::TokenType Ark::Lexer::FindType(const std::string_view& target)
{
    if(Ark::Lexer::IsInteger(target)) return Ark::TokenType::LITERAL_INT;
    if(Ark::Lexer::IsFloat(target)) return Ark::TokenType::LITERAL_FLOAT;
    if(Ark::Lexer::IsBoolean(target)) return Ark::TokenType::LITERAL_BOOL;
    if(Ark::Lexer::IsChar(target)) return Ark::TokenType::LITERAL_CHAR;
    if(Ark::Lexer::IsKeyword(target)) return Ark::TokenType::KEYWORD;
    if(Ark::Lexer::IsOpArithmetic(target)) return Ark::TokenType::OP_ARITHMETIC;
    if(Ark::Lexer::IsOpLogic(target)) return Ark::TokenType::OP_LOGICAL;
    if(Ark::Lexer::IsOpComparison(target)) return Ark::TokenType::OP_COMPARISON;
    if(Ark::Lexer::IsOpBitwise(target)) return Ark::TokenType::OP_BITWISE;
    if(Ark::Lexer::IsIdentifier(target)) return Ark::TokenType::IDENTIFIER;

    return Ark::TokenType::UNKNOWN;
}

void Ark::Lexer::PopulateHashsets()
{
    this->keywords = {
        Ark::KEYWORDS::TI8, Ark::KEYWORDS::TI16, Ark::KEYWORDS::TI32, Ark::KEYWORDS::TI64, 
        Ark::KEYWORDS::TUI8, Ark::KEYWORDS::TUI16, Ark::KEYWORDS::TUI32, Ark::KEYWORDS::TUI64,
        Ark::KEYWORDS::TF32, Ark::KEYWORDS::TF64, Ark::KEYWORDS::TBOOL, Ark::KEYWORDS::TVOID, 
        Ark::KEYWORDS::TSTRING, Ark::KEYWORDS::TRETURN, Ark::KEYWORDS::TINTERNAL, Ark::KEYWORDS::TVAR,
        Ark::KEYWORDS::TCONSTEXPR, Ark::KEYWORDS::TEND, Ark::KEYWORDS::TEXTENDS, Ark::KEYWORDS::TFUN,
        Ark::KEYWORDS::TMODULE, Ark::KEYWORDS::TIMPORT, Ark::KEYWORDS::TIF, Ark::KEYWORDS::TELIF,
        Ark::KEYWORDS::TELSE, Ark::KEYWORDS::TFOR, Ark::KEYWORDS::TWHILE, Ark::KEYWORDS::TENUM,
        Ark::KEYWORDS::TIN, Ark::KEYWORDS::TOF, Ark::KEYWORDS::TEXTERN, Ark::KEYWORDS::TTRUE,
        Ark::KEYWORDS::TFALSE, Ark::KEYWORDS::TSTRUCT
    };

    this->op_arithmetic = {
        Ark::OP_ARITHMETIC::ADD, Ark::OP_ARITHMETIC::DIV, 
        Ark::OP_ARITHMETIC::MULT, Ark::OP_ARITHMETIC::SUB
    };
    
    this->op_logic = { Ark::OP_LOGICAL::OR, Ark::OP_LOGICAL::AND, Ark::OP_LOGICAL::NOT };
    
    this->op_comparison = {
        Ark::OP_COMPARISON::EQ, Ark::OP_COMPARISON::DIFF, 
        Ark::OP_COMPARISON::GT, Ark::OP_COMPARISON::LT,
        Ark::OP_COMPARISON::GTEQ, Ark::OP_COMPARISON::LTEQ,
    };
    
    this->op_bitwise = {
        Ark::OP_BITWISE::OR, Ark::OP_BITWISE::AND, 
        Ark::OP_BITWISE::XOR, Ark::OP_BITWISE::NOT,
        Ark::OP_BITWISE::LSHIFT, Ark::OP_BITWISE::RSHIFT,
    };
}

bool Ark::Lexer::IsDigit(const std::string_view& target, uint8_t max_dots)
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

bool Ark::Lexer::IsInteger(const std::string_view& target)
{
    return Ark::Lexer::IsDigit(target, 0);
}

bool Ark::Lexer::IsFloat(const std::string_view& target)
{
    if(!Ark::Lexer::IsDigit(target, 1)) return false;
    if(target.front() == '.' || target.back() == '.') return false;
    return true;
}

bool Ark::Lexer::IsBoolean(const std::string_view& target)
{
    return target == Ark::KEYWORDS::TTRUE || target == Ark::KEYWORDS::TFALSE;
}

bool Ark::Lexer::IsChar(const std::string_view& target)
{
    if(target.size() < 3) return false;
    if(target.front() != '\'' || target.back() != '\'') return false;
    if(target.size() == 3) return target[1] != '\\';
    if(target.size() == 4) return target[1] == '\\';

    return true;
}

bool Ark::Lexer::IsKeyword(const std::string_view& target)
{
    return this->keywords.find(target) != this->keywords.end();
}

bool Ark::Lexer::IsOpArithmetic(const std::string_view& target)
{
    return this->op_arithmetic.find(target) != this->op_arithmetic.end();
}

bool Ark::Lexer::IsOpLogic(const std::string_view& target)
{
    return this->op_logic.find(target) != this->op_logic.end();
}

bool Ark::Lexer::IsOpComparison(const std::string_view& target)
{
    return this->op_comparison.find(target) != this->op_comparison.end();
}

bool Ark::Lexer::IsOpBitwise(const std::string_view& target)
{
    return this->op_bitwise.find(target) != this->op_bitwise.end();
}

uint8_t Ark::Lexer::IsDelimiter(size_t pos)
{
    auto safe_range = (pos + 1) < source.size();
    if(safe_range && source.substr(pos, 2) == Ark::DELIMITER::ARROW) return 2;
    if(safe_range && source.substr(pos, 2) == Ark::DELIMITER::SCOPEACCESS) return 2;
    if(safe_range && source.substr(pos, 2) == Ark::DELIMITER::ADDEQ) return 2;
    if(safe_range && source.substr(pos, 2) == Ark::DELIMITER::SUBEQ) return 2;
    if(safe_range && source.substr(pos, 2) == Ark::DELIMITER::MULTEQ) return 2;
    if(safe_range && source.substr(pos, 2) == Ark::DELIMITER::DIVEQ) return 2;
    if(source[pos] == Ark::DELIMITER::COMMA[0]) return 1;
    if(source[pos] == Ark::DELIMITER::COLON[0]) return 1;
    if(source[pos] == Ark::DELIMITER::LPARAN[0]) return 1;
    if(source[pos] == Ark::DELIMITER::RPARAN[0]) return 1;
    if(source[pos] == Ark::DELIMITER::LBRACE[0]) return 1;
    if(source[pos] == Ark::DELIMITER::RBRACE[0]) return 1;
    if(source[pos] == Ark::DELIMITER::LBRACKET[0]) return 1;
    if(source[pos] == Ark::DELIMITER::RBRACKET[0]) return 1;
    if(source[pos] == Ark::DELIMITER::SEMICOLON[0]) return 1;

    return 0;
}

bool Ark::Lexer::IsIdentifier(const std::string_view& target)
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
