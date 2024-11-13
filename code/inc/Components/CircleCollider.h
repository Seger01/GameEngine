#pragma once

#include "Collider.h"

class CircleCollider : public Collider
{
    public:
        CircleCollider();
        ~CircleCollider();

        std::unique_ptr<Component> clone() const override;

    private:
        float mRadius;
};