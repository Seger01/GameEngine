/**
 * @file Camera.cpp
 * @brief Implementation of the camera class
 */
#include "Camera.h"

/**
 * @brief Constructor. Sets the width and height to 800*600 by default.
 */
Camera::Camera()
	: GameObject(), mBackgroundColor(Color(0, 0, 0)), mWidth(800), mHeight(600), mViewport(FRect{0, 0, 1, 1})
{
}

void Camera::setBackgroundColor(const Color& aColor) { mBackgroundColor = aColor; }

Color Camera::getBackgroundColor() const { return mBackgroundColor; }

void Camera::setWidth(int aWidth) { mWidth = aWidth; }

int Camera::getWidth() const { return mWidth; }

void Camera::setHeight(int aHeight) { mHeight = aHeight; }

int Camera::getHeight() const { return mHeight; }

/**
 * @brief Get the coordinate of the camera's origin (top left corner) in game units.
 *
 * @return Coordinate of the camera's origin.
 */
Vector2 Camera::getOrigin() const
{
	// Get the centre's offset (half of the total width and height)
	Vector2 offset{getWidth() / 2.0f, getHeight() / 2.0f};
	return getTransform().position - offset;
}

void Camera::setViewport(const FRect& viewport) { mViewport = viewport; }

FRect Camera::getViewport() const { return mViewport; }
