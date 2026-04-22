#include <memory>
#include "CoreCompile.hpp"
#include "FileHandler.hpp"
#include "Lexer.hpp"

Ark::CoreCompile::CoreCompile(std::shared_ptr<Ark::ModuleResolver> module_manager): module_manager(module_manager) { }

void Ark::CoreCompile::Compile(const std::string& path)
{
    auto file_descriptor = Ark::FileHandler::GetFileContent(path);
    if(file_descriptor->source_code.empty()) return;

    auto token_manager = std::make_unique<Ark::Lexer>(file_descriptor);
    token_manager->Tokenize();
}
    
void Ark::CoreCompile::OnModuleReady()
{

}

