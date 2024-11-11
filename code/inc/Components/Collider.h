#pragma once

#include "Transform.h"
#include "Component.h"

class Collider : public Component
{
public:
    Collider();
    virtual ~Collider();

    // Getter and setter methods for transform
    Transform getTransform() const;
    void setTransform(const Transform& transform);

private:
    Transform mTransform;
};