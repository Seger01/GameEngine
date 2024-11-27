#pragma once

#include "CollisionFilters.h"
#include "Component.h"
#include "Transform.h"
#include "vector"

class Collider : public Component {
public:
    Collider();
    virtual ~Collider();

    virtual std::unique_ptr<Component> clone() const override;

    // Getter and setter methods for transform
    Transform getTransform() const;
    void setTransform(const Transform& transform);

private:
    Transform mTransform;
};
