#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Vector2.h"

struct Transform {
    Vector2 position; // Position in the world
    float rotation;   // Rotation angle in degrees
    Vector2 scale;    // Scale factor along the X and Y axes

    // Constructors
    Transform() : position(0, 0), rotation(0), scale(1, 1) {}

    Transform(Vector2 aPosition) : position(aPosition), rotation(0), scale(Vector2(0, 0)) {}
    Transform(Vector2 aPosition, float aRotation, Vector2 aScale)
        : position(aPosition), rotation(aRotation), scale(aScale) {}

    // Overload the += operator for Transform
    Transform operator+=(const Transform& other) const {
        return Transform(position + other.position, // Sum positions (uses Vector2 +)
                         rotation + other.rotation, // Sum rotations
                         scale + other.scale        // Sum scales (uses Vector2 +)
        );
    }

    // Overload the + operator for Transform
    Transform operator+(const Transform& other) const {
        return Transform(position + other.position, // Sum positions (uses Vector2 +)
                         rotation + other.rotation, // Sum rotations
                         scale + other.scale        // Sum scales (uses Vector2 +)
        );
    }

    // Helper functions to modify the transform
    void translate(const Vector2& delta) {
        position.x += delta.x;
        position.y += delta.y;
    }

    void rotate(float deltaRotation) { rotation += deltaRotation; }

    void scaleBy(const Vector2& scaleFactor) {
        scale.x *= scaleFactor.x;
        scale.y *= scaleFactor.y;
    }
};

#endif
