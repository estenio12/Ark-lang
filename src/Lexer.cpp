#include "Lexer.hpp"
#include "Definitions.hpp"
#include "Output.hpp"

std::unique_ptr<TokenManager> Lexer::Tokenize()
{
    auto tokens = std::make_unique<TokenManager>(file_descriptor);

    int64_t line = 1;
    int64_t col  = 0;
    uint16_t start_rect = 0;

    char letter;
    size_t source_size = file_descriptor->source_code.size();
    for(int i = 0; i < source_size; i++)
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
            Output::Print("Whiespace");
            start_rect = i + 1;
            continue;
        }

        auto delimiter = IsDelimitir(i);
        if(delimiter > 0)
        {
            // # Build token with first rect.
            this->BuildToken(start_rect, i, line, col);

            // # Build token with delimiter found.
            this->BuildToken(i, i + delimiter, line, col);
            start_rect = ++i;
            continue;
        }
    }

    return tokens;
}

uint8_t Lexer::IsDelimitir(int i)
{
    auto safe_range = (i + 1) < source.size();
    if(safe_range && source.substr(i, 2) == DELIMITER::ARROW) return 2;
    if(safe_range && source.substr(i, 2) == DELIMITER::SCOPEACCESS) return 2;
    if(source[i] == DELIMITER::COMMA[0]) return 1;
    if(source[i] == DELIMITER::COLON[0]) return 1;
    if(source[i] == DELIMITER::LPARAN[0]) return 1;
    if(source[i] == DELIMITER::RPARAN[0]) return 1;
    if(source[i] == DELIMITER::LBRACE[0]) return 1;
    if(source[i] == DELIMITER::RBRACE[0]) return 1;
    if(source[i] == DELIMITER::LBRACKET[0]) return 1;
    if(source[i] == DELIMITER::RBRACKET[0]) return 1;
    if(source[i] == DELIMITER::SEMICOLON[0]) return 1;

    return 0;
}

void Lexer::BuildToken(uint16_t start_rect, uint16_t end_rect, int64_t line, int64_t col)
{
    Token token;
    token.col_start = start_rect;
    token.col_end = end_rect;
    token.line = line;
    token.content = this->GetLexeme(start_rect, end_rect);

    Output::Print("\n>>\n");
    Output::Print(token.content);
    Output::Print("\n>>\n");
}

std::string_view Lexer::GetLexeme(size_t start, size_t end)
{
    return this->source.substr(start, end - start);
}

