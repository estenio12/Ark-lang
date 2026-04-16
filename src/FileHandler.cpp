#include <filesystem>
#include <fstream>
#include "FileHandler.hpp"
#include "Output.hpp"

std::shared_ptr<FileDescriptor> FileHandler::GetFileContent(const std::string& path)
{
    if(path.empty())
        Output::ThrowFatalError("Source code file path is required");

    auto absolute_path = FileHandler::PathResolver(path);
    
    std::ifstream in(absolute_path, std::ios::binary | std::ios::ate);

    if(!in.is_open())
        Output::ThrowFatalError("File cannot be read: " + absolute_path);

    std::streamsize size = in.tellg();
    
    auto file_descriptor = std::make_shared<FileDescriptor>();
    file_descriptor->absolute_path = absolute_path;
    file_descriptor->file_size = static_cast<uint64_t>(size);

    if(size > 0) 
    {
        in.seekg(0, std::ios::beg);
        file_descriptor->source_code.resize(size);
        if(!in.read(file_descriptor->source_code.data(), size))
            Output::ThrowFatalError("Error reading content from: " + absolute_path);
    }

    in.close();
    return file_descriptor; 
}

std::string FileHandler::PathResolver(const std::string& path)
{
    try 
    {
        return std::filesystem::canonical(path).string();
    } 
    catch (const std::filesystem::filesystem_error& e) 
    {
        Output::ThrowFatalError("Could not resolve path: " + path);
    }
}


