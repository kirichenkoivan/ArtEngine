#include "../include/engine/Tools/fileLoader.h"

std::string readFile(const std::string &filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    return buffer.str();
}

std::filesystem::path findFile(const std::string &fileName, const std::filesystem::path &searchDirectory = "FileSystem/")
{
    for (const auto &entry : std::filesystem::recursive_directory_iterator(searchDirectory))
    {
        if (entry.is_regular_file() && entry.path().filename() == fileName)
        {
            return entry.path();
        }
    }
    return std::filesystem::path();
}