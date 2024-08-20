#ifndef XMLREADER_H
#define XMLREADER_H

#include <iostream>
#include "tinyxml2.h"
#include <filesystem>
#include <string>

namespace fs = std::filesystem;
using namespace tinyxml2;

void process_xml_file(const fs::path& file_path) {
    XMLDocument doc;
    XMLError result = doc.LoadFile(file_path.c_str());

    if (result != XML_SUCCESS) {
        std::cerr << "Failed to load file: " << file_path << " - " << doc.ErrorStr() << std::endl;
        return;
    }

    // Получение корневого элемента
    XMLElement* root = doc.FirstChildElement("GameEntity");
    if (!root) {
        std::cerr << "Root element not found in file: " << file_path << std::endl;
        return;
    }

    // Перебор дочерних элементов
    for (XMLElement* child = root->FirstChildElement(); child; child = child->NextSiblingElement()) {
        std::cout << "Node name: " << child->Name() << std::endl;

        // Чтение атрибутов элемента
        for (const XMLAttribute* attr = child->FirstAttribute(); attr; attr = attr->Next()) {
            std::cout << "Attribute name: " << attr->Name() << ", value: " << attr->Value() << std::endl;
        }

        // Чтение текстового содержимого элемента
        if (child->GetText() != nullptr) {
            std::cout << "Node text: " << child->GetText() << std::endl;
        }
    }
}

void find_and_process_xml_files(const fs::path& directory) {
    try {
        if (fs::exists(directory) && fs::is_directory(directory)) {
            for (const auto& entry : fs::recursive_directory_iterator(directory)) {
                if (fs::is_regular_file(entry) && entry.path().extension() == ".xml") {
                    std::cout << "Processing file: " << entry.path() << std::endl;
                    process_xml_file(entry.path());
                }
            }
        } else {
            std::cerr << "Directory does not exist or is not a directory: " << directory << std::endl;
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "General error: " << e.what() << std::endl;
    }
}

#endif
