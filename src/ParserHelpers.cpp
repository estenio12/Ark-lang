#include "Parser.hpp"

Ark::Parser::PRIORITY Ark::Parser::GetPriority(const Token& token) 
{
    if (token.type == TokenType::OP_ARITHMETIC)
        return op_arith_precedency[token.content];
    
    if (precedency.count(token.type))
        return precedency[token.type];

    return PRIORITY::LVL_1;
}

void Ark::Parser::Expect(const std::string& token_content, std::string_view msg)
{
    auto current_token = this->tokens->Consume();
    if(current_token.content != token_content) this->PushTokenError(current_token, msg);
}

void Ark::Parser::PushTokenError(const Ark::Token& token, std::string_view msg)
{
    std::string buffer;
    buffer.reserve(250);

    buffer.append(this->tokens->GetFilePath());
    buffer.append("(Col ");
    buffer.append(std::to_string(token.col));
    buffer.append(", Ln ");
    buffer.append(std::to_string(token.line));
    buffer.append("): ");
    buffer.append(msg);

    Ark::Logs::Push(Ark::Logs::LogStatus::LOG_ERROR, buffer);
}
