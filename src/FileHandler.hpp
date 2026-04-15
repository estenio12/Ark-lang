#pragma once
#include <string>

class FileHandler
{
    private:
        std::string file_path;
        std::string file_content;

    public:
        FileHandler(std::string_view path);
        std::string_view GetContent() const;
        static std::string PathResolver(const std::string& path);
};
