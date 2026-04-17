#include "Lexer.hpp"
#include "Definitions.hpp"

std::unique_ptr<TokenManager> Lexer::Tokenize()
{
    auto tokens = std::make_unique<TokenManager>(file_descriptor);

    int64_t line = 1;
    int64_t col = -1;
    uint16_t start_rect = 0, end_rect = 0;

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
            i--;
            continue;
        }

        auto delimiter = IsDelimitir(i);
        if(letter == WHITESPACE || delimiter > 0)
        {
            i += delimiter - 1;
            end_rect = i;
        }
    }
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


void Lexer::BuildToken(uint16_t start_pos, uint16_t end_pos, int64_t line, int64_t col)
{
    
}



