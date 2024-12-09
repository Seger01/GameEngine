/**
 * @file Particle.h
 * @brief Defines a particle for use in particle emitters.
 */

#pragma once

#include <vector>

#include "Global/Color.h"
#include "Transform.h"
#include "Vector2.h"

/**
 * @class Particle
 * @brief Represents a particle in a particle emitter.
 */
class Particle
{
public:
	Particle(const Vector2& aPosition = Vector2(0, 0), const Vector2& aVelocity = Vector2(0, 0),
			 float aAcceleration = 0, int aLifeTime = 0, int aMaxLifeTime = 0, const Vector2& aSize = Vector2(0, 0),
			 const Vector2& aEndSize = Vector2(0, 0), float aRotation = 0, float aAngularVelocity = 0,
			 float aAngularAcceleration = 0, const std::vector<Color>& aColorGradient = std::vector<Color>());

	void update();

	Vector2 getPosition() const;
	float getLifeTime() const;
	Vector2 getSize() const;
	float getRotation() const;
	Color getColor() const;

private:
	Color calculateColor() const;
	Color calculateInterpolatedColor() const;
	Color getNearestColor() const;

private:
	Vector2 mPosition;
	Vector2 mVelocity;
	float mAcceleration;

	int mMaxLifeTime;
	float mLifeTimeRemainingSec;
	int mInitialLifeTime;

	Vector2 mStartSize;
	Vector2 mSize;
	Vector2 mEndSize;

	float mRotation;
	float mAngularVelocity;
	float mAngularAcceleration;
	std::vector<Color> mColorGradient;

	bool mInterpolateColor;
};
