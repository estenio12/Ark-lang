#pragma once
#include <string>
#include <map>
#include <memory>
#include <vector>
#include <functional>
#include <mutex>
#include "AST.hpp"
#include "SymbolTable.hpp"

namespace ModuleManager
{
    enum class EModuleState { WAITING, PROCESSING, READY, FAIL };
    
    class ModuleResolutionCaller
    {
        public:
            std::function<void()> listener;
            std::string owner_path;
            std::string import_path;
            std::vector<std::string> dependency_stack;
    };

    class ModuleResolutionState
    {
        private:
            mutable std::mutex mtx_liteners;

        private:
            std::vector<std::shared_ptr<ModuleResolutionCaller>> listeners;

        public:
            EModuleState state= EModuleState::WAITING;
            std::shared_ptr<SymbolTable> symbol_table;
            std::vector<std::string> messages;
            std::string import_path;

        public:
            ModuleResolutionState() = default;

        public:
            void UpdateState(EModuleState pstate);
            void AssignListener(std::shared_ptr<ModuleResolutionCaller> caller);
            bool IsReadyCalledTo(std::shared_ptr<ModuleResolutionCaller> caller) const;
    };

    class Manager
    {
        private:
            std::mutex mtx_module_history;

        private:
            // # Path, resolution-state reference.
            std::map<std::string, std::shared_ptr<ModuleResolutionState>> module_history;
            // # Stored AST processed by ImportResolution.
            std::map<std::string, std::unique_ptr<AST>> module_ast;

        public:
            Manager() = default;

        public:
            void ImportResolution(std::shared_ptr<ModuleResolutionCaller> caller);
            std::shared_ptr<ModuleResolutionState> GetResolutionState(const std::string& path);
            AST* GetASTByPath(const std::string& path) const;

        private:
            std::string FormatStack(const std::vector<std::string>& stack);
    };
}
