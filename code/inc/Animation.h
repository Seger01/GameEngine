/**
 * @file Animation.h
 * @brief Animation class header file.
 */

#pragma once

#include <iostream>
#include <memory>
#include <vector>

#include "Components/Sprite.h"

/**
 * @class Animation
 * @brief Animation class.
 */
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

	Transform getTransform() const;
	void setTransform(const Transform& aNewTransform);

	const Sprite& getFrame(int aFrameIndex) const;
	const Sprite& getCurrentFrame() const;

	Color getColorFilter() const;
	void setColorFilter(const Color& aColor);

	int getTimeBetweenFrames() const;
	void setTimeBetweenFrames(int aTimeBetweenFrames);

	void setFlipX(bool state);
	bool getFlipX() const;

	void setFlipY(bool state);
	bool getFlipY() const;

	bool isLooping() const;
	void setIsLooping(bool aIsLooping);

	int getFrameCount() const;

	void setLayer(int aLayer);
	int getLayer() const;

private:
	/// @brief The transform of the animation.
	Transform mTransform;

	/// @brief The animation frames.
	std::vector<std::unique_ptr<Sprite>> mAnimationFrames; // Unique pointers for Sprite objects

	/// @brief true if the antimation is flipped horizontally.
	bool mFlipX;

	/// @brief true if the antimation is flipped vertically.
	bool mFlipY;
	/// @brief the time between frames of the animation
	int mTimeBetweenFrames;
	/// @brief if the animation is looping
	bool mIsLooping;
	/// @brief the layer of the animation
	int mLayer;
};
