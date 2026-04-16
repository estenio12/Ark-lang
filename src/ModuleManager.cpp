#include "ModuleManeger.hpp"
#include "Output.hpp"

namespace ModuleManager
{
    #pragma region MODULE MANAGER

    void Manager::ImportResolution(std::shared_ptr<ModuleResolutionCaller> caller)
    {
        {
            std::lock_guard<std::mutex> lock(this->mtx_module_history);
            auto module = this->module_history.find(caller->import_path);

            if(module == this->module_history.end())
            {
                auto new_state = std::make_shared<ModuleResolutionState>();
                new_state->AssignListener(caller);
                new_state->import_path = caller->import_path;
                this->module_history.emplace(caller->import_path, new_state);
            }
            else
            {
                auto resolve_state = module->second;

                for (const auto& ancestor : caller->dependency_stack) 
                {
                    if (ancestor == caller->import_path) 
                    {
                        Output::ThrowFatalError("Circular dependency chain: " + 
                            this->FormatStack(caller->dependency_stack) + " -> " + caller->import_path);
                    }
                }

                if(resolve_state->state == EModuleState::FAIL)
                    Output::ThrowFatalError("Required module cannot be read because it has fail load: " + resolve_state->import_path);
                
                if(resolve_state->state == EModuleState::PROCESSING ||
                   resolve_state->state == EModuleState::WAITING    )
                {
                    resolve_state->AssignListener(caller);
                    return;
                }

                // # If Module ready to consume only retorn the call.
                if(caller->listener) caller->listener();
            }
        }
    }

    std::shared_ptr<ModuleResolutionState> Manager::GetResolutionState(const std::string& path)
    {
        auto it = this->module_history.find(path);
        if(it != this->module_history.end()) return it->second;
        Output::ThrowFatalError("Resolution state not found: " + path);
    }

    AST* Manager::GetASTByPath(const std::string& path) const
    {
        auto it = this->module_ast.find(path);
        if(it != this->module_ast.end()) return it->second.get();
        Output::ThrowFatalError("AST not found in Module-Manager: " + path);
    }

    std::string Manager::FormatStack(const std::vector<std::string>& stack)
    {
        std::string circular_history = "";
        for(int i = 0; i < stack.size(); i++) 
        { 
            if(i > 0) circular_history += " -> ";
            circular_history += stack[i]; 
        }

        return circular_history;
    }

    #pragma endregion

    #pragma region MODULO RESOLUTION STATE

    void ModuleResolutionState::UpdateState(EModuleState pstate)
    {
        std::vector<std::shared_ptr<ModuleResolutionCaller>> to_notify;

        {
            std::lock_guard<std::mutex> lock(this->mtx_liteners);
            this->state = pstate;

            if(pstate == EModuleState::READY || pstate == EModuleState::FAIL)
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

    bool ModuleResolutionState::IsReadyCalledTo(std::shared_ptr<ModuleResolutionCaller> caller) const
    {
        std::lock_guard<std::mutex> lock(this->mtx_liteners);
        for(auto& it : this->listeners)
            if(it->import_path == caller->import_path && it->owner_path == caller->owner_path) return true;

        return false;
    }

    #pragma endregion
};