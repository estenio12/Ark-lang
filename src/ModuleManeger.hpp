#pragma once
#include <string>
#include <map>
#include <memory>
#include <vector>
#include "AST.hpp"
#include "SymbolTable.hpp"

namespace ModuleManager
{
    class ModuleResolutionState;
    enum class EModuleState { WAITING, PROCESSING, READY, ERROR };

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

    class ModuleResolutionState
    {
        public:
            std::shared_ptr<SymbolTable> symbol_table;
            EModuleState state= EModuleState::WAITING;
    };
}
