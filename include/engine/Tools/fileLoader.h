#ifndef FILE_LOADER_H
#define FILE_LOADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

std::filesystem::path findFile(const std::string& fileName, const std::filesystem::path& searchDirectory);

std::string readFile(const std::string& filePath);


#endif