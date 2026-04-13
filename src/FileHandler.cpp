#include <filesystem>
#include <fstream>
#include "FileHandler.hpp"
#include "Output.hpp"

FileHandler::FileHandler(std::string_view file_path): file_path(file_path)
{
    if(this->file_path.empty())
        Output::ThrowFatalError("Source code file path is required");

    if(!std::filesystem::exists(std::filesystem::path(this->file_path)))
        Output::ThrowFatalError("Source code cannot be founded: " + this->file_path);

    std::ifstream in(this->file_path, std::ios::binary | std::ios::ate);
    
    if(!in.is_open())
        Output::ThrowFatalError("File cannot be read: " + this->file_path);

    std::streamsize size = in.tellg();
    if(size <= 0) return;
    in.seekg(0, std::ios::beg);

    this->file_content.resize(size);

    if(!in.read(this->file_content.data(), size))
        Output::ThrowFatalError("Error reading content from: " + this->file_path);
    
    in.close();
}

std::string_view FileHandler::GetContent() const
{
    return this->file_content;
}

