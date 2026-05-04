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
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstdlib>
#include "Output.hpp"
#include "Tools.hpp"

std::filesystem::path OUTPUT_FILE_NAME = "tmp-build.test";
std::filesystem::path CURRENT_BASE_PATH = "";

std::filesystem::path GetDirByPath(const std::string& path);
void RunTestFileTxt(const std::string& path);
bool RunTest(const std::string& ark_file, const std::string& exp_file);

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        Ark::Output::Print("Expected a path to test-list.txt");
        exit(-1);
    }

    // # Test file processing.
    std::string path_test_file = std::string(argv[1]);

    // # Test execute
    RunTestFileTxt(path_test_file);

    Ark::Output::Print("Test-Runner was completed.");
    return EXIT_SUCCESS;
}

std::filesystem::path GetDirByPath(const std::string& path)
{
    if(path.empty()) return path;

    std::filesystem::path lpath(path);
    std::filesystem::path dir = lpath.parent_path();
    return dir;
}

void RunTestFileTxt(const std::string& path)
{
    std::ifstream file(path, std::ios::binary);

    if(file.is_open())
    {
        std::string test_line;
        uint8_t line_jump_counter = 0;
        uint8_t line_counter = 0;
        while(std::getline(file, test_line))
        {
            line_counter++;
            // # Ignore the first lines becouse they are metadata.
            if(line_jump_counter > 0)
            {
                line_jump_counter--;
                continue;
            }

            // # Ignore blank lines and commented lines.
            if(test_line.empty() || test_line.size() <= 1) continue;
            if(test_line.size() > 1 && test_line[0] == '#') continue;

            if(test_line.starts_with("basepath"))
            {
                auto chunks = Ark::Tools::Str::Split(test_line, '=');

                if(chunks.size() < 2)
                    Ark::Output::PrintError("Malformed line at: " + std::to_string(line_counter));
                else
                    CURRENT_BASE_PATH = chunks[1];

                continue;
            }

            // # index 0 => .ark file to test with your arguments.
            // # index 1 => .exp file to compare in test-runner.
            auto paths = Ark::Tools::Str::Split(test_line, '=');

            if(paths.size() < 2)
            {
                Ark::Output::PrintError("Malformed line at: " + std::to_string(line_counter));
                continue;
            }

            auto result = RunTest(paths[0], paths[1]);

            Ark::Output::Print("[ ", false);
            if(result)
                Ark::Output::PrintSuccess("PASS", false);
            else
                Ark::Output::PrintError("FAIL", false);
            Ark::Output::Print(" ] - " + paths[0]);
        }

        file.close();
    }
    else
    {
        Ark::Output::PrintError("Cannot read test-file.txt: " + path);
        exit(-1);
    }
}

bool RunTest(const std::string& ark_file, const std::string& exp_file)
{
    if(ark_file.empty())
    {
        Ark::Output::PrintError("Ark file cannot be readed: " + ark_file);
        return false;
    }
    if(exp_file.empty())
    {
        Ark::Output::PrintError("Exp file cannot be readed: " + exp_file);
        return false;
    }

    auto OUTPUT_PATH = CURRENT_BASE_PATH / OUTPUT_FILE_NAME;
    auto ark_file_path = CURRENT_BASE_PATH / ark_file;
    auto exp_file_path = CURRENT_BASE_PATH / exp_file; // Garantir que o basepath se aplica ao .exp
    
    #if _WIN32
        std::string cmd = "arkc.exe ";
    #else
        std::string cmd = "./arkc ";
    #endif
    
    cmd += "--no-timer --f " + ark_file_path.string() + " --plex " + OUTPUT_PATH.string();

    int result = std::system(cmd.c_str());

    #if _WIN32
        if(result != 0) 
    #else
        if(result == -1 || WEXITSTATUS(result) != 0)
    #endif
    {
        Ark::Output::PrintError("Compiler process with error or crashed!");
        return false;
    }

    auto read_first_line = [](const std::filesystem::path& p) -> std::string {
        std::ifstream file(p, std::ios::binary);
        std::string line;
        if(file.is_open()) {
            std::getline(file, line);
            file.close();
        }
        line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
        return line;
    };

    std::string target_hash = read_first_line(OUTPUT_PATH);
    std::string exp_hash    = read_first_line(exp_file_path);

    if(target_hash.empty() || exp_hash.empty())
    {
        return false;
    }

    return target_hash == exp_hash;
}


