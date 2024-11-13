#ifndef BUTTON_H
#define BUTTON_H

#include "UIObject.h"

// define bounding box
// define interactable

struct BoundingBox {
    Vector2 topLeft;
    Vector2 bottomRight;

    bool contains(Vector2 aPoint) {
        return aPoint.x >= topLeft.x && aPoint.x <= bottomRight.x && aPoint.y >= topLeft.y && aPoint.y <= bottomRight.y;
    }
};

class Button : public UIObject {
public:
    Button();
    ~Button();
    void OnClick();

    void setWidth(float aWidth);
    float getWidth() const;

    void setHeight(float aHeight);
    float getHeight() const;

    void setInteractable(bool aInteractable);
    bool interactable() const;

    BoundingBox getBoundingBox() const;

private:
    float mWidth;
    float mHeight;

    bool mInteractable;
};

#endif // BUTTON_H
