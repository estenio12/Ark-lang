#include <memory>
#include "CoreCompile.hpp"
#include "FileHandler.hpp"
#include "Lexer.hpp"

CoreCompile::CoreCompile(std::shared_ptr<ModuleManager::Manager> module_manager): module_manager(module_manager) { }

void CoreCompile::Compile(const std::string& path)
{
    auto file_descriptor = FileHandler::GetFileContent(path);
    if(file_descriptor->source_code.empty()) return;

    auto token_manager = std::make_unique<Lexer>(file_descriptor);
    token_manager->Tokenize();
}
    
void CoreCompile::OnModuleReady()
{

}

