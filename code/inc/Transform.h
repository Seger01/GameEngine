#pragma once
#include "Point.h"

// class Transform
// {
//     public:
//         Transform();
//         ~Transform();
//         void setPosition(Point position);
//         Point getPosition();
//         void setRotation(float rotation);
//         float getRotation();
//         void setScale(Point scale);
//         Point getScale();
//     private:
//         Point mPosition;
//         float mRotation;
//         Point mScale;
// };

typedef struct {
    Point position;
    float rotation;
    Point scale;

} Transform;
