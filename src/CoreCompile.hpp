#pragma once

#include <string>
#include <memory>
#include "ModuleManeger.hpp"

class CoreCompile
{
    private:
        std::string current_module_load;

    private:
        std::shared_ptr<ModuleManager::Manager> module_manager;

    public:
        CoreCompile(std::shared_ptr<ModuleManager::Manager> module_manager);

    public:
        void Compile(const std::string& path);
    
    private:
        void OnModuleReady();
};

