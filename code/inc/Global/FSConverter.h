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
    std::string executablePath();
    std::string mResourceDir;
    static std::string mCachedResourceDir;
};
