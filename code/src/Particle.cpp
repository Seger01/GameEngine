#include "Particle.h"

#include <iostream>

#include "Time.h"

Particle::Particle(const Vector2& aPosition, const Vector2& aVelocity, float aAcceleration, int aLifeTime,
				   int aMaxLifeTime, const Vector2& aSize, const Vector2& aSizeShift, float aRotation,
				   float aAngularVelocity, float aAngularAcceleration, const std::vector<Color>& aColorGradient)
	: mPosition(aPosition), mVelocity(aVelocity), mAcceleration(aAcceleration), mMaxLifeTime(aMaxLifeTime),
	  mSize(aSize), mEndSize(aSizeShift), mRotation(aRotation), mAngularVelocity(aAngularVelocity),
	  mAngularAcceleration(aAngularAcceleration), mColorGradient(aColorGradient), mStartSize(aSize),
	  mInitialLifeTime(aLifeTime), mInterpolateColor(true)
{
	mLifeTimeRemainingSec = static_cast<double>(aLifeTime) / 1000;
}

void Particle::update()
{
	mPosition.x += mVelocity.x * Time::deltaTime;
	mPosition.y += mVelocity.y * Time::deltaTime;

	mVelocity.x += mAcceleration * Time::deltaTime;
	mVelocity.y += mAcceleration * Time::deltaTime;
	mLifeTimeRemainingSec -= Time::deltaTime;

	if (mLifeTimeRemainingSec <= 0)
	{
		mLifeTimeRemainingSec = 0;
	}

	float percentage = (static_cast<float>(mMaxLifeTime) / 1000) -
					   (mLifeTimeRemainingSec + (static_cast<float>(mMaxLifeTime - mInitialLifeTime) / 1000));

	mSize.x = mStartSize.x + ((mEndSize.x - mStartSize.x) * percentage);
	mSize.y = mStartSize.y + ((mEndSize.y - mStartSize.y) * percentage);

	if (mSize.x < 0)
	{
		mSize.x = 0;
		mLifeTimeRemainingSec = 0;
	}

	if (mSize.y < 0)
	{
		mSize.y = 0;
		mLifeTimeRemainingSec = 0;
	}

	mRotation += mAngularVelocity * Time::deltaTime;
	mAngularVelocity += mAngularAcceleration * Time::deltaTime;
}

Vector2 Particle::getPosition() const { return mPosition; }

float Particle::getLifeTime() const { return mLifeTimeRemainingSec; }

Vector2 Particle::getSize() const { return mSize; }

float Particle::getRotation() const { return mRotation; }

Color Particle::getColor() const
{
	if (mColorGradient.size() == 0)
	{
		return Color(255, 255, 255, 255);
	}
	else if (mColorGradient.size() == 1)
	{
		return mColorGradient[0];
	}
	else
	{
		return calculateColor();
	}

	return mColorGradient[0];
}

Color Particle::calculateColor() const
{
	if (mInterpolateColor)
	{
		return calculateInterpolatedColor();
	}
	else
	{
		return getNearestColor();
	}
}

Color Particle::calculateInterpolatedColor() const
{
	// Calculate the percentage of life time remaining
	float percentage = (static_cast<float>(mMaxLifeTime) / 1000) -
					   (mLifeTimeRemainingSec + (static_cast<float>(mMaxLifeTime - mInitialLifeTime) / 1000));

	// Calculate the index of the color gradient
	int index = static_cast<int>(percentage * (mColorGradient.size() - 1));

	// Handle edge case where percentage is 1 (fully alive)
	if (index >= mColorGradient.size() - 1)
	{
		return mColorGradient.back(); // Return the last color if fully alive
	}

	// Get the two colors to interpolate between
	Color startColor = mColorGradient[index];
	Color endColor = mColorGradient[index + 1];

	// Calculate the interpolation factor
	float t = (percentage * (mColorGradient.size() - 1)) - index; // Fractional part

	// Perform linear interpolation between startColor and endColor
	Color interpolatedColor;
	interpolatedColor.r = static_cast<uint8_t>(startColor.r + t * (endColor.r - startColor.r));
	interpolatedColor.g = static_cast<uint8_t>(startColor.g + t * (endColor.g - startColor.g));
	interpolatedColor.b = static_cast<uint8_t>(startColor.b + t * (endColor.b - startColor.b));
	interpolatedColor.a = static_cast<uint8_t>(startColor.a + t * (endColor.a - startColor.a));

	return interpolatedColor;
}

Color Particle::getNearestColor() const
{
	// Calculate the percentage of life time remaining
	float percentage = (static_cast<float>(mMaxLifeTime) / 1000) -
					   (mLifeTimeRemainingSec + (static_cast<float>(mMaxLifeTime - mInitialLifeTime)));

	// Calculate the index of the color gradient
	int index = static_cast<int>(percentage * (mColorGradient.size() - 1));

	// Calculate the color based on the gradient
	Color color = mColorGradient[index];

	return color;
}
