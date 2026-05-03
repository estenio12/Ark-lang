/*
 * Copyright 2026 Ark-lang Authors
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "ModuleManeger.hpp"
#include "Output.hpp"

//#region MODULE MANAGER

void Ark::ModuleResolver::ImportResolution(std::shared_ptr<Ark::ModuleResolutionCaller> caller)
{
    {
        std::lock_guard<std::mutex> lock(this->mtx_module_history);
        auto module = this->module_history.find(caller->import_path);

        if(module == this->module_history.end())
        {
            auto new_state = std::make_shared<Ark::ModuleResolutionState>();
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
                    Ark::Output::ThrowFatalError("ModuleManager", "Circular dependency chain: " + 
                        this->FormatStack(caller->dependency_stack) + " -> " + caller->import_path);
                }
            }

            if(resolve_state->state == Ark::EModuleState::FAIL)
                Ark::Output::ThrowFatalError("ModuleManager", "Required module cannot be read because it has fail load: " + resolve_state->import_path);
            
            if(resolve_state->state == Ark::EModuleState::PROCESSING ||
                resolve_state->state == Ark::EModuleState::WAITING    )
            {
                resolve_state->AssignListener(caller);
                return;
            }

            // # If Module ready to consume only retorn the call.
            if(caller->listener) caller->listener();
        }
    }
}

std::shared_ptr<Ark::ModuleResolutionState> Ark::ModuleResolver::GetResolutionState(const std::string& path)
{
    auto it = this->module_history.find(path);
    if(it != this->module_history.end()) return it->second;
    Ark::Output::ThrowFatalError("ModuleManager", "Resolution state not found: " + path);
}

Ark::AST* Ark::ModuleResolver::GetASTByPath(const std::string& path) const
{
    auto it = this->module_ast.find(path);
    if(it != this->module_ast.end()) return it->second.get();
    Ark::Output::ThrowFatalError("ModuleManager", "AST not found in Module-Manager: " + path);
}

std::string Ark::ModuleResolver::FormatStack(const std::vector<std::string>& stack)
{
    std::string circular_history = "";
    for(size_t i = 0; i < stack.size(); i++) 
    { 
        if(i > 0) circular_history += " -> ";
        circular_history += stack[i]; 
    }

    return circular_history;
}

// #endregion

//#region MODULO RESOLUTION STATE

void Ark::ModuleResolutionState::UpdateState(Ark::EModuleState pstate)
{
    std::vector<std::shared_ptr<Ark::ModuleResolutionCaller>> to_notify;

    {
        std::lock_guard<std::mutex> lock(this->mtx_liteners);
        this->state = pstate;

        if(pstate == Ark::EModuleState::READY || pstate == Ark::EModuleState::FAIL)
            to_notify = std::move(this->listeners);
    } 

    for(const auto& caller : to_notify)
    {
        if(caller && caller->listener) caller->listener();
    }
}

void Ark::ModuleResolutionState::AssignListener(std::shared_ptr<Ark::ModuleResolutionCaller> caller)
{
    std::lock_guard<std::mutex> lock(this->mtx_liteners);
    this->listeners.push_back(caller);
}

bool Ark::ModuleResolutionState::IsReadyCalledTo(std::shared_ptr<Ark::ModuleResolutionCaller> caller) const
{
    std::lock_guard<std::mutex> lock(this->mtx_liteners);
    for(auto& it : this->listeners)
        if(it->import_path == caller->import_path && it->owner_path == caller->owner_path) return true;

    return false;
}

//#endregion