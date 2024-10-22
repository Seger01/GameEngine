#include "Components/Sprite.h"
#include <iostream>

Sprite::Sprite() : mSprite(""), mFlipX(false), mFlipY(false) {}

Sprite::~Sprite() {
    // Cleanup resources if necessary
}

void Sprite::render() {
    // This is where you'd add the actual rendering logic, for example, using SDL or another graphics API.
    // For now, we will just simulate rendering by printing to the console.
    std::cout << "Rendering sprite: " << mSprite << (mFlipX ? " [Flipped horizontally]" : "")
              << (mFlipY ? " [Flipped vertically]" : "") << std::endl;
}

// Additional functionality such as color, sorting layer, and order-in-layer would go here.
