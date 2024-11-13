#pragma once

#include <filesystem>
#include <limits.h>
#include <stdexcept>
#include <string>
#include <unistd.h>

class FSConverter {
public:
    FSConverter(std::string ResourceDir = "");

    std::string getResourcePath(const std::string& resourceName, bool aCheckExists = false);

private:
    std::string findResourcesFolder();
    bool isFilesystemAccessible(const std::filesystem::path& path);
    std::string executablePath();
    std::string resourceDir;
};