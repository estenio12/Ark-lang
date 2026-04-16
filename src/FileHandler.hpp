#pragma once
#include <string>
#include <memory>

struct FileDescriptor
{
    std::string absolute_path;
    std::string source_code;
    uint64_t file_size = 0;
};

class FileHandler
{
    public:
        static std::shared_ptr<FileDescriptor> GetFileContent(const std::string& path);
        
    private:
        static std::string PathResolver(const std::string& path);
};
