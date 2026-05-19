#include "Parser.hpp"

std::unique_ptr<Ark::Ast::AstBranch> Ark::Parser::Parse()
{
    auto ast = std::make_unique<Ark::Ast::AstBranch>();
    ast->branches.push_back(this->ParseBlockScope());
    return ast;
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
                if(current_token.content == Ark::KEYWORDS::TINTERNAL ||
                   current_token.content == Ark::KEYWORDS::TVAR      ||
                   current_token.content == Ark::KEYWORDS::TCONST    ||
                   current_token.content == Ark::KEYWORDS::TFUN      ||    
                   current_token.content == Ark::KEYWORDS::TSTRUCT   )
                {
                    block_scope->body.push_back(this->ParseModifier());
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

std::unique_ptr<Ark::Ast::AstNode> Ark::Parser::ParseModifier()
{
    bool is_internal = false;
    auto current_token = this->tokens->Peek(0);
    
    if(current_token.content == Ark::KEYWORDS::TINTERNAL)
    {
        is_internal = true;
        this->tokens->Advance();
        current_token = this->tokens->Peek(0);
    }

    if(current_token.content == Ark::KEYWORDS::TVAR)
        return this->ParseVariableDeclaration(is_internal);

    std::string message = "The 'internal' modifier can only be applied to 'var', 'const', 'fun', or 'struct' declarations, but found '" + current_token.content + "'.";
    this->PushTokenError(current_token, std::move(message));

    this->tokens->Advance();
    return nullptr;
}

std::unique_ptr<Ark::Ast::VariableDeclaration> Ark::Parser::ParseVariableDeclaration(bool is_internal)
{
    auto current_token = this->tokens->Peek(0);
    auto ast_node = std::make_unique<Ark::Ast::VariableDeclaration>();
    ast_node->is_internal = is_internal;
    ast_node->col = current_token.col;
    ast_node->line = current_token.line;
    ast_node->source_file = this->tokens->GetFilePath();

    this->Expect(Ark::KEYWORDS::TVAR, "Expected keyword 'var'");

    auto token_id = this->tokens->Consume();
    if(token_id.type == Ark::TokenType::IDENTIFIER)
    {
        ast_node->identifier = token_id.content;
    }
    else
    {
        this->PushTokenError(token_id, "Expected a 'identifier' after 'var' declaration, but found '" +token_id.content+ "'.");
        while(!this->tokens->IsAtEnd())
        {
            auto tmp = this->tokens->Consume();
            if(tmp.content == Ark::DELIMITER::SEMICOLON) break; 
        }
    }

    this->Expect(Ark::DELIMITER::COLON, "Expected the delimiter ':' after 'identifier' definition." );

    ast_node->type_def = this->ParseTypeIdentifier();

    return ast_node;
}

std::unique_ptr<Ark::Ast::TypeIdentifier> Ark::Parser::ParseTypeIdentifier()
{
    auto token_type = this->tokens->Consume();
    auto ast_node = std::make_unique<Ark::Ast::TypeIdentifier>();
    ast_node->col = token_type.col;
    ast_node->line = token_type.line;
    ast_node->source_file = this->tokens->GetFilePath();
    
    if(token_type.type == Ark::TokenType::KEYWORD        ||
       token_type.type == Ark::TokenType::LITERAL_INT    || 
       token_type.type == Ark::TokenType::LITERAL_FLOAT  || 
       token_type.type == Ark::TokenType::LITERAL_BOOL   || 
       token_type.type == Ark::TokenType::LITERAL_CHAR   || 
       token_type.type == Ark::TokenType::LITERAL_STRING )
    {
        ast_node->col  = token_type.col;
        ast_node->line = token_type.line;
        ast_node->source_file = this->tokens->GetFilePath();
        
        if(token_type.content == Ark::KEYWORDS::TFUN)
        {
            ast_node->is_function_type = true;
            ast_node->base_type = this->ParseTypeFunction();
        }
    }
    else
    {
        this->PushTokenError(token_type, "Expected a type identifier, but found '" +token_type.content+ "'.");
        return ast_node;
    }

    // # Array definition
    if(this->tokens->Peek(0).content == Ark::DELIMITER::LBRACKET)
    {
        this->Expect(Ark::DELIMITER::LBRACKET, "Expected the delimiter '[' after the identifier.");
        auto token = this->tokens->Consume();
        if(token.type == Ark::TokenType::LITERAL_INT) 
        {
            ast_node->is_array_type = true;
            ast_node->array_range = std::stoi(token.content);
        }
        else
        {
            this->PushTokenError(token_type, "Expected a literal int to define array range, but found '" +token.content+ "'.");
            return ast_node;
        }
        this->Expect(Ark::DELIMITER::RBRACKET, "Expected the delimiter ']' after the type identifier.");
    }

    return ast_node;
}

std::unique_ptr<Ark::Ast::TypeFunction> Ark::Parser::ParseTypeFunction()
{
    auto ast_node = std::make_unique<Ark::Ast::TypeFunction>();
    this->Expect(Ark::DELIMITER::LPARAN, "Expected the delimiter '(' after 'fun' declaration.");
    
    if(this->tokens->Peek(0).content != Ark::DELIMITER::RPARAN)
        ast_node->type_list = this->ParseTypeList();

    this->Expect(Ark::DELIMITER::RPARAN, "The delimiter '(' was open but never close.");
    this->Expect(Ark::DELIMITER::ARROW, "Expected the delimiter '->' after the function parameter declaration.");
    ast_node->type_def = this->ParseTypeIdentifier();

    return ast_node;
}

std::unique_ptr<Ark::Ast::TypeList> Ark::Parser::ParseTypeList()
{
    auto ast_node = std::make_unique<Ark::Ast::TypeList>();
    ast_node->source_file = this->tokens->GetFilePath();

    ast_node->type_list.push_back(this->ParseTypeIdentifier());
    
    if(this->tokens->Peek(0).content == Ark::DELIMITER::COMMA)
    {
        this->tokens->Advance();
        while(!this->tokens->IsAtEnd())
        {
            ast_node->type_list.push_back(this->ParseTypeIdentifier());
            if(this->tokens->Peek(0).content != Ark::DELIMITER::COMMA) break;
        }
    }

    return ast_node;
}

