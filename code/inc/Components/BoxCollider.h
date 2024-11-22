#pragma once

#include "Collider.h"

class BoxCollider : public Collider {
public:
    BoxCollider(std::string aTag = "defaultBoxCollider");
    virtual ~BoxCollider();

    std::unique_ptr<Component> clone() const override;

    // Getter and setter methods for width, height, and rotation
    float getWidth() const;
    void setWidth(float width);

    float getHeight() const;
    void setHeight(float height);

    float getRotation() const;
    void setRotation(float rotation);

    bool isTrigger() const;
    void setTrigger(bool isTrigger);

private:
    bool mIsTrigger = false;
    float mWidth;
    float mHeight;
    float mRotation;
};
