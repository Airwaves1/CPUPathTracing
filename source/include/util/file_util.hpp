#pragma once

#include <string>

#ifdef PROJECT_ROOT_PATH
const std::string ROOT_DIR = std::string(PROJECT_ROOT_PATH) + "/";
#define OUTPUT_PATH PROJECT_ROOT_PATH "/output.ppm"
#define RESOURCE_PATH PROJECT_ROOT_PATH "/resources/"
#define MODEL_PATH PROJECT_ROOT_PATH "/resources/models/"
#endif