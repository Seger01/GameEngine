#pragma once

#include "Collider.h"
#include "TileMapParser.h"


class BoxCollider : public Collider
{
public:
    BoxCollider();
    virtual ~BoxCollider();

    // Getter and setter methods for width, height, and rotation
    float getWidth() const;
    void setWidth(float width);

    float getHeight() const;
    void setHeight(float height);

    float getRotation() const;
    void setRotation(float rotation);
    void setTransformFromColliderData(const ColliderData& colliderData);


private:
    float mWidth;
    float mHeight;
    float mRotation;
};