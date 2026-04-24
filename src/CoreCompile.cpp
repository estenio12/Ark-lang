#include <memory>
#include "CoreCompile.hpp"
#include "FileHandler.hpp"
#include "Lexer.hpp"

Ark::CoreCompile::CoreCompile(std::shared_ptr<Ark::ModuleResolver> module_manager): module_manager(module_manager) { }

void Ark::CoreCompile::Compile(const std::string& path)
{
    auto file_descriptor = Ark::FileHandler::GetFileContent(path);
    if(file_descriptor->source_code.empty()) return;

    auto lexer = std::make_unique<Ark::Lexer>(file_descriptor);
    auto token_manager = lexer->Tokenize();
    token_manager->printTokens();
}
    
void Ark::CoreCompile::OnModuleReady()
{

}

