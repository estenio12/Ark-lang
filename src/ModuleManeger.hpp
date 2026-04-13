#pragma once
#include <string>
#include <map>
#include <memory>
#include <vector>
#include <functional>
#include "AST.hpp"
#include "SymbolTable.hpp"

namespace ModuleManager
{
    enum class EModuleState { WAITING, PROCESSING, READY, ERROR };
    
    class ModuleResolutionState
    {
        public:
            EModuleState state= EModuleState::WAITING;
            std::shared_ptr<SymbolTable> symbol_table;
            std::vector<std::function<void>> listeners;

        public:
            void UpdateState(EModuleState pstate);
    };

    class ModuleManager
    {
        private:
            // # Path, resolution-state reference.
            std::map<std::string, std::shared_ptr<ModuleResolutionState>> module_history;
            // # Stored AST processed by ImportREsolution.
            std::vector<std::unique_ptr<AST>> module_ast;

        public:
            ModuleManager() = default;

        public:
            void ImportResolution(std::string impot_path);
    };
}
