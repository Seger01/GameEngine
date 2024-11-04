#include "FSConverter.h"
#include <iostream>

FSConverter::FSConverter(std::string ResourceDir) {
    resourceDir = ResourceDir;
    if (resourceDir.empty()) {
        resourceDir = findResourcesFolder();
    }

    if (resourceDir.empty()) {
        std::cerr << "Error: Could not locate /Resources folder!" << std::endl;
        throw std::runtime_error("Resources folder not found.");
    } else {
        std::cout << "Resources folder located at: " << resourceDir << std::endl;
    }
}

std::string FSConverter::findResourcesFolder() {
    std::filesystem::path execPath = executablePath();

    // Try to find the Resources folder in a few places relative to the executable
    std::filesystem::path potentialPaths[] = {
        execPath / "Resources",         // Resources in the same directory as the executable
        execPath / "../Resources",      // Resources in the parent directory of the executable
        execPath / "../../Resources",   // Resources two levels up (in case of a bin/debug layout)
        execPath / "../../../Resources" // Resources two levels up (in case of a bin/debug layout)
    };

    for (const auto& path : potentialPaths) {
        if (std::filesystem::exists(path)) {
            return path.string();
        }
    }

    return ""; // Resources folder not found
}

std::string FSConverter::getResourcePath(const std::string& resourceName) {
    std::filesystem::path fullPath = std::filesystem::path(resourceDir) / resourceName;

    if (!std::filesystem::exists(fullPath)) {
        std::cerr << "Error: Resource " << resourceName << " does not exist at " << fullPath.string() << std::endl;
        return "";
    }

    return fullPath.string();
}

std::string FSConverter::executablePath() {
    char path[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", path, PATH_MAX);
    if (count == -1) {
        throw std::runtime_error("Unable to determine the executable path.");
    }
    return std::filesystem::path(path).parent_path().string();
}
