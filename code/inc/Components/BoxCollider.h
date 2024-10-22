#pragma once

#include "Collider.h"

class BoxCollider : public Collider
{
    public:
        BoxCollider();
        ~BoxCollider();
    private:
        float mWidth;
        float mHeight;
        float mRotation;
};