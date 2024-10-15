#include <iostream>
#include "FSConverter.h"

int main() {
    try {
        FSConverter converter;

        // Look for a file called "resource1.txt" in the located Resources folder
        std::string resourcePath = converter.getResourcePath("resource1.txt");

        if (!resourcePath.empty()) {
            std::cout << "Full path to resource: " << resourcePath << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
