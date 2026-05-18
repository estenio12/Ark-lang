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
#include <string>
#include <cstdint>
#include <vector>
#include <utility>

namespace Ark::Logs
{
    enum class LogStatus : uint8_t { LOG_SUCCESS, LOG_ERROR, LOG_WARNING };
    
    struct Log
    {
        LogStatus status;
        std::string message;

        public:
            Log(LogStatus status, std::string message):
                status(status), message(message) {}
    };

    inline bool HasCriticLogs = false;
    inline std::vector<Log> GlobalStack;

    inline void Push(LogStatus status, std::string message)
    {
        if(status == LogStatus::LOG_ERROR) HasCriticLogs = true;
        GlobalStack.emplace_back(status, std::move(message));
    }

    inline void ClearLogs()
    {
        HasCriticLogs = false;
        GlobalStack.clear();
    }
}