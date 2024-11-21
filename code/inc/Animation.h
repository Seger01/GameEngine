#ifndef ANIMATION_H
#define ANIMATION_H

#include <iostream>
#include <memory>
#include <vector>

#include "Sprite.h"

class Animation : public Component {
public:
    Animation(std::vector<Sprite*> aAnimationFrames, int aTimeBetweenFrames, bool aIsLooping = false);
    ~Animation();

    // Rule of Five
    Animation(const Animation& other);                // Copy constructor
    Animation& operator=(const Animation& other);     // Copy assignment operator
    Animation(Animation&& other) noexcept;            // Move constructor
    Animation& operator=(Animation&& other) noexcept; // Move assignment operator

    // Override the clone method
    std::unique_ptr<Component> clone() const override {
        return std::make_unique<Animation>(*this); // Uses the copy constructor of Sprite
    }

    Transform getTransform();
    void setTransform(Transform aNewTransform);

    Sprite* getFrame(int aFrameIndex);
    Sprite* getCurrentFrame();

    Color getColorFilter();
    void setColorFilter(Color aColor);

    int getTimeBetweenFrames();
    void setTimeBetweenFrames(int aTimeBetweenFrames);

    void setFlipX(bool state);
    bool getFlipX();

    void setFlipY(bool state);
    bool getFlipY();

    bool isLooping();
    void setIsLooping(bool aIsLooping);

    int getFrameCount();

    void setLayer(int aLayer);
    int getLayer();

private:
    Transform mTransform;

    std::vector<std::unique_ptr<Sprite>> mAnimationFrames; // Unique pointers for Sprite objects

    bool mFlipX;
    bool mFlipY;
    int mTimeBetweenFrames;
    bool mIsLooping;
    int mLayer = 0;
};

#endif
