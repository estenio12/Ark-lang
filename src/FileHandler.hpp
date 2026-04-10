#pragma once
#include <vector>
#include <fstream>

class FileHandler
{
    private:
        std::string base_dir;
        std::string filepath;

    private:
        std::fstream file;

    public:
        FileHandler() = default;
        FileHandler(std::string_view path);
    
    public:
        void LoadFilePath(std::string_view path);
        std::vector<std::string> GetNextBucket();
    
    private:
        void CheckIfValidPath();
};
