#include "Parser.hpp"

Ark::Parser::PRIORITY Ark::Parser::GetPriority(const Token& token) 
{
    if (token.type == TokenType::OP_ARITHMETIC)
        return op_arith_precedency[token.content];
    
    if (precedency.count(token.type))
        return precedency[token.type];

    return PRIORITY::LVL_1;
}

