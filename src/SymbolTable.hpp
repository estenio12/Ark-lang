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
#pragma once
#include <vector>
#include <string>
#include <memory>
#include <cstdint>

namespace Ark
{
    enum class ESTRowType : uint8_t
    {
        TI8,
        TI16,
        TI32,
        TI64,
        TUI8,
        TUI16,
        TUI32,
        TUI64,
        TF32,
        TF64,
        TBOOL,
        TSTRING,
        TVOID,
        TSTRUCT,
        TFUNCTION
    };

    struct TypeSignature 
    {
        ESTRowType category;
        std::string type_name;
        
        std::shared_ptr<TypeSignature> array_type; 
        
        std::vector<std::shared_ptr<TypeSignature>> params;
        std::shared_ptr<TypeSignature> return_type;
        
        std::vector<std::shared_ptr<TypeSignature>> generic_args;
    };

    struct STRow 
    {
        std::string id;
        std::string module_id;
        uint32_t deep;
        bool is_const = false;
        bool is_ready = false; 
        bool is_internal = false; 
        TypeSignature type;

        std::string GetFullName() const 
        {
            return module_id.empty() ? id : module_id + "::" + id;
        }
    };

    using Scope = std::unordered_map<std::string, std::shared_ptr<STRow>>;

    class SymbolTable 
    {
        private:
            std::vector<Scope> stack;

        public:
            SymbolTable() { stack.emplace_back(); }

            void EnterScope() { stack.emplace_back(); }

            void ExitScope() { if (stack.size() > 1) stack.pop_back(); }

            bool ExistsAnywhere(const std::string& id) 
            {
                for (auto it = stack.rbegin(); it != stack.rend(); ++it) 
                {
                    if (it->find(id) != it->end()) return true;
                }
                return false;
            }

            // Retorna o símbolo se ele existir em algum nível da pilha (do mais profundo para o global)
            std::shared_ptr<STRow> Lookup(const std::string& id)
            {
                for (auto it = stack.rbegin(); it != stack.rend(); ++it) 
                {
                    auto search = it->find(id);
                    if (search != it->end()) return search->second;
                }
                return nullptr;
            }

            bool Declare(std::shared_ptr<STRow> row) 
            {
                if (ExistsAnywhere(row->id)) return false;
                
                row->deep = static_cast<uint32_t>(stack.size() - 1);
                
                stack.back()[row->id] = row;
                return true;
            }
    };
}
