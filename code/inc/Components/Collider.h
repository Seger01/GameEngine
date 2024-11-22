#pragma once

#include "Transform.h"
#include "Component.h"

class Collider : public Component
{
public:
    Collider(std::string aTag = "defaultCollider");
    virtual ~Collider();

    virtual std::unique_ptr<Component> clone() const override;

    // Getter and setter methods for transform
    Transform getTransform() const;
    void setTransform(const Transform& transform);

private:
    Transform mTransform;
};