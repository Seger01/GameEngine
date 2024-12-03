#pragma once

#include <iostream>
#include <memory>
#include <vector>

#include "Sprite.h"

class Animation : public Component
{
public:
	Animation(std::vector<std::reference_wrapper<Sprite>> aAnimationFrames, int aTimeBetweenFrames,
			  bool aIsLooping = false, const std::string& aTag = "defaultAnimation");
	~Animation();

	// Rule of Five
	Animation(const Animation& other);				  // Copy constructor
	Animation& operator=(const Animation& other);	  // Copy assignment operator
	Animation(Animation&& other) noexcept;			  // Move constructor
	Animation& operator=(Animation&& other) noexcept; // Move assignment operator

	// Override the clone method
	std::unique_ptr<Component> clone() const override;

	Transform getTransform();
	void setTransform(const Transform& aNewTransform);

	Sprite& getFrame(int aFrameIndex);
	Sprite& getCurrentFrame();

	Color getColorFilter();
	void setColorFilter(const Color& aColor);

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
	int mLayer;
};