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
#include <memory>
#include "CoreCompile.hpp"
#include "FileHandler.hpp"
#include "Lexer.hpp"

Ark::CoreCompile::CoreCompile(std::shared_ptr<Ark::ModuleResolver> module_manager): module_manager(module_manager) { }

void Ark::CoreCompile::Compile(const std::string& path)
{
    auto file_descriptor = Ark::FileHandler::GetFileContent(path);
    if(file_descriptor->source_code.empty()) return;

    auto lexer = std::make_unique<Ark::Lexer>(file_descriptor);
    auto token_manager = lexer->Tokenize();
}
    
void Ark::CoreCompile::OnModuleReady()
{

}

