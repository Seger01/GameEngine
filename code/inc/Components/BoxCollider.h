#pragma once

#include "Collider.h"

class BoxCollider : public Collider {
public:
    BoxCollider();
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

    filterCategory getFilterCategory() const;
    void setFilterCategory(filterCategory category);

    std::vector<filterCategory> getCollideWith() const;
    void setCollideWith(std::vector<filterCategory> aCollideWith);

private:
    bool mIsTrigger;
    float mWidth;
    float mHeight;
    float mRotation;
    std::vector<filterCategory> mCollideWith;
    filterCategory mFilterCategory;
};
