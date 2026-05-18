#include "Parser.hpp"

Ark::Parser::PRIORITY Ark::Parser::GetPriority(const Token& token) 
{
    if (token.type == TokenType::OP_ARITHMETIC)
        return op_arith_precedency[token.content];
    
    if (precedency.count(token.type))
        return precedency[token.type];

    return PRIORITY::LVL_1;
}

std::unique_ptr<Ark::Ast::AstBranch> Ark::Parser::Parse()
{
    auto ast = std::make_unique<Ark::Ast::AstBranch>();
    ast->branches.push_back(this->ParseBlockScope());
    return ast;
}

void Ark::Parser::Consume(const std::string& token_content, std::string_view msg)
{
    auto current_token = this->tokens->Consume();
    if(current_token.content != token_content)
    {
        std::string buffer;
        buffer.reserve(250);

        buffer.append(this->tokens->GetFilePath());
        buffer.append("(Col ");
        buffer.append(std::to_string(current_token.col));
        buffer.append(", Ln ");
        buffer.append(std::to_string(current_token.line));
        buffer.append("): ");
        buffer.append(msg);

        Ark::Logs::Push(Ark::Logs::LogStatus::LOG_ERROR, std::move(buffer));
    }
}

std::unique_ptr<Ark::Ast::BlockScope> Ark::Parser::ParseBlockScope()
{
    auto block_scope = std::make_unique<Ark::Ast::BlockScope>();

    while(!tokens->IsAtEnd())
    {
        auto current_token = tokens->Peek(0);

        switch(current_token.type)
        {
            case Ark::TokenType::KEYWORD:
            {
                if(current_token.content == Ark::KEYWORDS::TVAR)
                {
                    block_scope->body.push_back(this->ParseVariableDeclaration());
                    continue;
                }

                if(current_token.content == Ark::KEYWORDS::TEND)
                {
                    tokens->Advance();
                    return block_scope;
                }
            }
            break;

            case Ark::TokenType::END_OF_FILE: return block_scope;
        }
    }

    return block_scope;
}

std::unique_ptr<Ark::Ast::VariableDeclaration> Ark::Parser::ParseVariableDeclaration()
{

}


