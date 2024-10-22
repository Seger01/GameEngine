#include "Renderer.h"
#include "Point.h"
#include <iostream>

Renderer::Renderer() {
    // Initialize the resolution to a default value, e.g., 800x600
    mResolution = Point(800, 600);
    std::cout << "Renderer created with default resolution 800x600." << std::endl;
}

Renderer::~Renderer() {
    std::cout << "Renderer destroyed." << std::endl;
}

void Renderer::render(const Scene* scene) {
    // Placeholder implementation for rendering a scene
    std::cout << "Rendering scene." << std::endl;
}

void Renderer::render(const Scene& scene, int layer) {
    // Placeholder implementation for rendering a specific layer of a scene
    std::cout << "Rendering scene layer " << layer << "." << std::endl;
}

void Renderer::clear() {
    // Placeholder implementation for clearing the renderer
    std::cout << "Clearing renderer." << std::endl;
}

void Renderer::setResolution(Point resolution) {
    mResolution = resolution;
    std::cout << "Resolution set to " << resolution.x << "x" << resolution.y << "." << std::endl;
}