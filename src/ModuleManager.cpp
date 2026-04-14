#include "ModuleManeger.hpp"
#include "Output.hpp"

namespace ModuleManager
{
    #pragma region MODULE MANAGER

    void ModuleManager::ImportResolution(std::shared_ptr<ModuleResolutionCaller> caller)
    {
        
    }

    std::shared_ptr<ModuleResolutionState> ModuleManager::GetResolutionState(const std::string& path)
    {
        auto it = this->module_history.find(path);
        if(it != this->module_history.end()) return it->second;
        Output::ThrowFatalError("Resolution state not found: " + path);
    }

    AST* ModuleManager::GetASTByPath(const std::string& path) const
    {
        auto it = this->module_ast.find(path);
        if(it != this->module_ast.end()) return it->second.get();
        Output::ThrowFatalError("AST not found in Module-Manager: " + path);
    }

    #pragma endregion

    #pragma region MODULO RESOLUTION STATE

    void ModuleResolutionState::UpdateState(EModuleState pstate)
    {
        std::vector<std::shared_ptr<ModuleResolutionCaller>> to_notify;

        {
            std::lock_guard<std::mutex> lock(this->mtx_liteners);
            this->state = pstate;

            if(pstate == EModuleState::READY || pstate == EModuleState::ERROR)
                to_notify = std::move(this->listeners);
        } 

        for(const auto& caller : to_notify)
        {
            if(caller && caller->listener) caller->listener();
        }
    }

    void ModuleResolutionState::AssignListener(std::shared_ptr<ModuleResolutionCaller> caller)
    {
        std::lock_guard<std::mutex> lock(this->mtx_liteners);
        this->listeners.push_back(caller);
    }

    #pragma endregion
};