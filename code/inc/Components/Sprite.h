#pragma once

#include <string>

#include "Component.h"
#include "Rect.h"
#include "Texture.h"
#include "Transform.h"

class Sprite : public Component {
public:
    Sprite(Texture* aTexture, int aWidth, int aHeight, Rect aSourceRect = Rect());
    ~Sprite();

    // Rule of Five
    Sprite(const Sprite& other);                // Copy constructor
    Sprite& operator=(const Sprite& other);     // Copy assignment operator
    Sprite(Sprite&& other) noexcept;            // Move constructor
    Sprite& operator=(Sprite&& other) noexcept; // Move assignment operator

    // Override the clone method
    std::unique_ptr<Component> clone() const override;

    Texture* getTexture();

    Transform getRelativePosition();
    void setRelativePosition(Transform aNewTransform);

    Rect getSource();
    void setSource(Rect aSourceRect);

    void setFlipX(bool state) { mFlipX = state; }
    bool getFlipX() { return mFlipX; }

    void setFlipY(bool state) { mFlipY = state; }
    bool getFlipY() { return mFlipY; }

    void setWidth(int aWidth) { mWidth = aWidth; }
    int getWidth() { return mWidth; }

    void setHeight(int aHeight) { mHeight = aHeight; }
    int getHeight() { return mHeight; }

    void setLayer(int aLayer) { mLayer = aLayer; }
    int getLayer() { return mLayer; }

private:
    Texture* mTexture = nullptr;
    Transform mTransform;

    Rect mSourceRect;

    std::string mSprite;
    float mWidth = 0;
    float mHeight = 0;
    //??? mColor;
    //??? mColor;
    bool mFlipX;
    bool mFlipY;

    int mLayer = 0;
};
