#include "../include/engine/Tools/xmlReader.h"


fs::path FindXMLFile(const std::string& fileName, const fs::path& searchDirectory) {
    for (const auto& entry : fs::recursive_directory_iterator(searchDirectory)) {
        if (entry.is_regular_file() && entry.path().filename() == fileName) {
            return entry.path();
        }
    }
    return fs::path(); // Вернуть пустой путь, если файл не найден
}

std::unique_ptr<tinyxml2::XMLDocument> LoadXMLFile(const fs::path& filePath) {
    auto doc = std::make_unique<tinyxml2::XMLDocument>();
    tinyxml2::XMLError result = doc->LoadFile(filePath.string().c_str());

    if (result != tinyxml2::XML_SUCCESS) {
        std::cerr << "Failed to load file: " << filePath << std::endl;
        return nullptr;
    }

    return doc;
}

std::vector<std::pair<std::string, std::string>> GetAttributesValues(const tinyxml2::XMLDocument& document, const std::vector<std::string>& attributes) {
    std::vector<std::pair<std::string, std::string>> attributesValues;
    
    const tinyxml2::XMLElement* element = document.FirstChildElement("GameEntity")->FirstChildElement("info");
    if (element) {
        for (const std::string& attrName : attributes) {
            const char* attrValue = element->Attribute(attrName.c_str());
            if (attrValue) {
                attributesValues.emplace_back(attrName, std::string(attrValue));
            } else {
                attributesValues.emplace_back(attrName, ""); // Пустая строка, если атрибут не найден
                std::cerr << "Attribute '" << attrName << "' not found in element." << std::endl;
            }
        }
    } else {
        std::cerr << "Tag not found" << std::endl;
    }

    return attributesValues;
}

std::vector<std::pair<std::string, std::string>> GetAttributesValues(tinyxml2::XMLElement* element, const std::vector<std::string>& attributes) {
    std::vector<std::pair<std::string, std::string>> attributesValues;
    
    for (const std::string& attrName : attributes) {
        const char* attrValue = element->Attribute(attrName.c_str());
        if (attrValue) {
            attributesValues.emplace_back(attrName, std::string(attrValue));
        } else {
            attributesValues.emplace_back(attrName, ""); // Пустая строка, если атрибут не найден
            std::cerr << "Attribute '" << attrName << "' not found in element." << std::endl;
        }
    }
    

    return attributesValues;
}

float XMLToFloat(const std::string value)
{
    return std::stof(value);
}

int XMLToInt(const std::string value)
{
    return std::stoi(value);
}

bool XMLToBool(const std::string value){
    if (value == "true"){
        return true;
    }
    else if (value == "false"){
        return false;
    }
    return false;
}

void XMLToGLloatArr(const std::string value, GLfloat(&array)[4])
{
    // Удаляем круглые скобки
    std::string cleanedStr = value.substr(1, value.size() - 2);

    // Используем stringstream для парсинга
    std::stringstream ss(cleanedStr);
    std::string token;

    int index = 0;

    // Разделяем строку по запятым
    while (std::getline(ss, token, ',')) {
        if (index >= 4) {
            throw std::out_of_range("Input string contains more than 4 elements.");
        }

        // Убираем лишние пробелы
        token.erase(std::remove_if(token.begin(), token.end(), ::isspace), token.end());
        
        // Конвертируем строку в GLfloat и добавляем в массив
        array[index++] = static_cast<GLfloat>(std::stof(token));
    }

    if (index != 4) {
        throw std::out_of_range("Input string does not contain exactly 4 elements.");
    }
}

const char* ConvertStringToCStr(const std::string& input) {
    return input.c_str();
}
