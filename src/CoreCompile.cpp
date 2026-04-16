#include <memory>
#include "CoreCompile.hpp"
#include "FileHandler.hpp"

CoreCompile::CoreCompile(std::shared_ptr<ModuleManager::Manager> module_manager): module_manager(module_manager) { }

void CoreCompile::Compile(const std::string& path)
{
    auto file_descriptor = FileHandler::GetFileContent(path);
    if(file_descriptor->source_code.empty()) return;

    
}
    
void CoreCompile::OnModuleReady()
{

}

