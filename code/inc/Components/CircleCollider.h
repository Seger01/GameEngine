#pragma once

#include "Collider.h"

class CircleCollider : public Collider
{
    public:
        CircleCollider();
        ~CircleCollider();
    private:
        float mRadius;
};