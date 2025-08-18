/**
 * @file Particle.h
 * @brief Defines a particle for use in particle emitters.
 */

#pragma once

#include <vector>

#include "Global/Color.h"
#include "Structs/Transform.h"
#include "Structs/Vector2.h"

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
	/// @brief The position of the particle.
	Vector2 mPosition;
	/// @brief The velocity of the particle.
	Vector2 mVelocity;
	/// @brief The acceleration of the particle.
	float mAcceleration;

	/// @brief The maximum lifetime of the particle.
	int mMaxLifeTime;
	/// @brief The remaining lifetime of the particle.
	float mLifeTimeRemainingSec;
	/// @brief The initial lifetime of the particle.
	int mInitialLifeTime;

	/// @brief The initial size of the particle.
	Vector2 mStartSize;
	/// @brief the current size of the particle.
	Vector2 mSize;
	/// @brief The end size of the particle.
	Vector2 mEndSize;

	/// @brief the current rotation of the particle.
	float mRotation;
	/// @brief the angular velocity of the particle.
	float mAngularVelocity;
	/// @brief the angular acceleration of the particle.
	float mAngularAcceleration;

	/// @brief the color gradient of the particle.
	std::vector<Color> mColorGradient;

	/// @brief if the colorgradient should be interpolated.
	bool mInterpolateColor;
};
