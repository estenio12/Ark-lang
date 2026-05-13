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
#include <memory>
#include <unordered_map>
#include "Definitions.hpp"
#include "TokenManager.hpp"
#include "AST.hpp"

namespace Ark
{
    class Parser
    {
        public:
            enum class PRIORITY : uint8_t
            {
                LVL_1 = 1,
                LVL_2 = 2,
                LVL_3 = 3,
                LVL_4 = 4,
                LVL_5 = 5,
                LVL_6 = 6
            };

            std::unordered_map<TokenType, PRIORITY> precedency =
            {
                {TokenType::OP_LOGICAL, PRIORITY::LVL_1},
                {TokenType::OP_COMPARISON, PRIORITY::LVL_2},
                {TokenType::OP_BITWISE, PRIORITY::LVL_3},
                {TokenType::OP_ARITHMETIC, PRIORITY::LVL_4},
                {TokenType::DELIMITER, PRIORITY::LVL_6},
            };

            std::unordered_map<std::string, PRIORITY> op_arith_precedency =
            {
                {std::string(Ark::OP_ARITHMETIC::ADD), PRIORITY::LVL_4},
                {std::string(Ark::OP_ARITHMETIC::SUB), PRIORITY::LVL_4},
                {std::string(Ark::OP_ARITHMETIC::MUL), PRIORITY::LVL_5},
                {std::string(Ark::OP_ARITHMETIC::DIV), PRIORITY::LVL_5},
                {std::string(Ark::OP_ARITHMETIC::MOD), PRIORITY::LVL_5},
            };

        public:
            Parser() = default;
        
            std::unique_ptr<Ark::Ast::AstBranch> Parse(std::unique_ptr<TokenManager> tokens);

        // # Helpers functions.
        private:
            PRIORITY GetPriority(const Ark::Token& token);

        // # Parser functions.
        private:
            
    };
}