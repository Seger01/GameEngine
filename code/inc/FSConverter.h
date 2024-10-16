#pragma once

#include <string>
#include <filesystem>
#include <unistd.h>
#include <limits.h>
#include <stdexcept>

class FSConverter {
public:
    FSConverter(std::string ResourceDir = "");

    std::string getResourcePath(const std::string& resourceName);

private:
    std::string findResourcesFolder();
    std::string executablePath();
    std::string resourceDir;
};