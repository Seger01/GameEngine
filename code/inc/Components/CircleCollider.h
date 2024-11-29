#ifndef CIRCLE_COLLIDER_H
#define CIRCLE_COLLIDER_H

#include "Collider.h"

class CircleCollider : public Collider {
public:
    CircleCollider(float aRadius = 0.0f, std::string aTag = "defaultCircleCollider");
    ~CircleCollider();

    float getRadius() const;
    void setRadius(float radius);

    std::unique_ptr<Component> clone() const override;

private:
    float mRadius;
};

#endif // CIRCLE_COLLIDER_H
