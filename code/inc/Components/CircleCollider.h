#ifndef CIRCLE_COLLIDER_H
#define CIRCLE_COLLIDER_H

#include "Collider.h"

class CircleCollider : public Collider {
public:
    CircleCollider();
    CircleCollider(float radius);
    ~CircleCollider();

    float getRadius() const;
    void setRadius(float radius);

    int getCollideCategory() const;
    void setCollideCategory(int category);

    std::vector<int> getCollideWithCategory() const;
    void setCollideWithCategory(std::vector<int> aCollideWith);

    std::unique_ptr<Component> clone() const override;

private:
    std::vector<int> mCollideWithCategory;
    int mCollideCategory;

    float mRadius;
};

#endif // CIRCLE_COLLIDER_H
