#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <cstdint>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <algorithm>
#include <chrono>
#include "../src/Output.hpp"

#ifdef _WIN32
    #define POPEN _popen
    #define PCLOSE _pclose
#else
    #define POPEN popen
    #define PCLOSE pclose
#endif

using Targets = std::vector<std::pair<std::string, std::string>>;

Targets LoadTestList(std::string target);
bool Expect(std::string target, std::string expect);

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        Ark::Output::PrintError("Expect a test-list.txt file", true);
        return EXIT_FAILURE;
    }

    auto target = LoadTestList(argv[1]);
    bool hasFail = false;

    for(const auto& it : target)
    {
        auto start = std::chrono::high_resolution_clock::now();
        if(Expect(it.first, it.second))
        {
            Ark::Output::Print("[ ");
            Ark::Output::PrintSuccess("PASS: ");
            Ark::Output::Print(" ] - " + it.first);
        }
        else
        {
            Ark::Output::Print("[ ");
            Ark::Output::PrintError("FAIL");
            Ark::Output::Print(" ] - "+ it.first);
            hasFail = true;
        }

        auto timer_end = std::chrono::high_resolution_clock::now();
        auto elipsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(timer_end - start);
        Ark::Output::Print(" - ");
        Ark::Output::PrintInfo(std::to_string(elipsed_time.count()) + "ms\n");
    }

    if(hasFail) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

Targets LoadTestList(std::string target)
{
    Targets targets;

    std::ifstream in(target, std::ios::in);

    if(in.is_open() && in.good())
    {
        std::string content;
        while(std::getline(in, content))
        {
            std::string buffer[2];
            uint8_t pointer = 0;
            for(const auto& letter : content)
            {
                if(letter == '=')
                {
                    pointer = 1;
                    continue;
                }

                buffer[pointer].push_back(letter);
            }

            targets.push_back(std::make_pair(buffer[0], buffer[1]));
            buffer[0].clear();
            buffer[1].clear();
            pointer = 0;
        }
    }
    else
        Ark::Output::ThrowFatalError("File cannot be read: " + target);

    in.close();

    return targets;
}

bool Expect(std::string target, std::string expect)
{
    std::string command = "../arkc.exe " + target + " 2>&1"; 
    std::string actual_output;
    char buffer[128];

    std::unique_ptr<FILE, decltype(&PCLOSE)> pipe(POPEN(command.c_str(), "r"), PCLOSE);
    
    if (!pipe) 
    {
        Ark::Output::PrintError("Erro ao executar arkc para o alvo: " + target);
        return false;
    }

    while (fgets(buffer, sizeof(buffer), pipe.get()) != nullptr) 
    {
        actual_output += buffer;
    }

    actual_output.erase(std::remove(actual_output.begin(), actual_output.end(), '\r'), actual_output.end());
    
    std::ifstream in(expect, std::ios::binary | std::ios::ate);
    if(!in.is_open()) return false;

    std::streamsize size = in.tellg();
    std::string expect_content;

    if(size > 0) 
    {
        expect_content.resize(size);
        in.seekg(0, std::ios::beg);
        in.read(&expect_content[0], size);
    }
    in.close();

    expect_content.erase(std::remove(expect_content.begin(), expect_content.end(), '\r'), expect_content.end());

    auto trim = [](std::string& s) {
        size_t last = s.find_last_not_of(" \n\r\t");
        if (last != std::string::npos) s.erase(last + 1);
        else s.clear();
    };

    trim(actual_output);
    trim(expect_content);

    return (actual_output == expect_content);
}

