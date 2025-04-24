#ifndef XMLREADER_H
#define XMLREADER_H

#include <iostream>
#include "tinyxml2.h"
#include <filesystem>
#include <string>
#include <vector>
#include <utility>
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <sstream>
#include <algorithm>
#include <stdexcept>

namespace fs = std::filesystem;

fs::path FindXMLFile(const std::string &fileName, const fs::path &searchDirectory);

std::unique_ptr<tinyxml2::XMLDocument> LoadXMLFile(const fs::path &filePath);

std::vector<std::pair<std::string, std::string>> GetAttributesValues(const tinyxml2::XMLDocument &document, const std::vector<std::string> &attributes);
std::vector<std::pair<std::string, std::string>> GetAttributesValues(tinyxml2::XMLElement *element, const std::vector<std::string> &attributes);

float XMLToFloat(const std::string value);
int XMLToInt(const std::string value);
bool XMLToBool(const std::string value);
void XMLToGLloatArr(const std::string value, GLfloat (&array)[4]);
const char *ConvertStringToCStr(const std::string &input);

#endif
