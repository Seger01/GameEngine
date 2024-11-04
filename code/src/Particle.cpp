#include "Particle.h"

#include <iostream>

#include "Time.h"

Particle::Particle()
    : velocity(Vector2(0, 0)), acceleration(0), lifeTimeRemaining(0), size(Vector2(0, 0)), endSize(Vector2(0, 0)),
      rotation(0), angularVelocity(0), angularAcceleration(0), colorGradient(std::vector<Color>()) {}

Particle::Particle(Vector2 aPosition, Vector2 aVelocity, float aAcceleration, int aLifeTime, int aMaxLifeTime,
                   Vector2 aSize, Vector2 aSizeShift, float aRotation, float aAngularVelocity,
                   float aAngularAcceleration, std::vector<Color> aColorGradient)
    : position(aPosition), velocity(aVelocity), acceleration(aAcceleration), maxLifeTime(aMaxLifeTime),
      lifeTimeRemaining(aLifeTime), size(aSize), endSize(aSizeShift), rotation(aRotation),
      angularVelocity(aAngularVelocity), angularAcceleration(aAngularAcceleration), colorGradient(aColorGradient) {

    startSize = aSize;
    initialLifeTime = aLifeTime;
}

Particle::~Particle() {}

void Particle::update() {
    position.x += velocity.x * Time::deltaTime;
    position.y += velocity.y * Time::deltaTime;

    velocity.x += acceleration * Time::deltaTime;
    velocity.y += acceleration * Time::deltaTime;
    lifeTimeRemaining -= Time::deltaTime;

    float percentage =
        static_cast<float>(maxLifeTime - (lifeTimeRemaining + (maxLifeTime - initialLifeTime))) / maxLifeTime;

    size.x = startSize.x + ((endSize.x - startSize.x) * percentage);
    size.y = startSize.y + ((endSize.y - startSize.y) * percentage);

    if (size.x < 0) {
        size.x = 0;
        lifeTimeRemaining = 0;
    }

    if (size.y < 0) {
        size.y = 0;
        lifeTimeRemaining = 0;
    }

    rotation += angularVelocity * Time::deltaTime;
    angularVelocity += angularAcceleration * Time::deltaTime;
}

Vector2 Particle::getPosition() { return position; }

float Particle::getLifeTime() { return lifeTimeRemaining; }

Vector2 Particle::getSize() { return size; }

float Particle::getRotation() { return rotation; }

Color Particle::getColor() {
    if (colorGradient.size() == 0) {
        return Color(255, 255, 255, 255);
    } else if (colorGradient.size() == 1) {
        return colorGradient[0];
    } else {
        return calculateColor();
    }

    return colorGradient[0];
}

Color Particle::calculateColor() {
    if (interpolateColor) {
        return calculateInterpolatedColor();
    } else {
        return getNearestColor();
    }
}

Color Particle::calculateInterpolatedColor() {
    // Calculate the percentage of life time remaining
    float percentage =
        static_cast<float>(maxLifeTime - (lifeTimeRemaining + (maxLifeTime - initialLifeTime))) / maxLifeTime;

    // Calculate the index of the color gradient
    int index = static_cast<int>(percentage * (colorGradient.size() - 1));

    // Handle edge case where percentage is 1 (fully alive)
    if (index >= colorGradient.size() - 1) {
        return colorGradient.back(); // Return the last color if fully alive
    }

    // Get the two colors to interpolate between
    Color startColor = colorGradient[index];
    Color endColor = colorGradient[index + 1];

    // Calculate the interpolation factor
    float t = (percentage * (colorGradient.size() - 1)) - index; // Fractional part

    // Perform linear interpolation between startColor and endColor
    Color interpolatedColor;
    interpolatedColor.r = static_cast<uint8_t>(startColor.r + t * (endColor.r - startColor.r));
    interpolatedColor.g = static_cast<uint8_t>(startColor.g + t * (endColor.g - startColor.g));
    interpolatedColor.b = static_cast<uint8_t>(startColor.b + t * (endColor.b - startColor.b));
    interpolatedColor.a = static_cast<uint8_t>(startColor.a + t * (endColor.a - startColor.a));

    return interpolatedColor;
}

Color Particle::getNearestColor() {
    // Calculate the percentage of life time remaining
    float percentage = static_cast<float>(maxLifeTime - lifeTimeRemaining) / maxLifeTime;

    // Calculate the index of the color gradient
    int index = static_cast<int>(percentage * (colorGradient.size() - 1));

    // Calculate the color based on the gradient
    Color color = colorGradient[index];

    return color;
}
