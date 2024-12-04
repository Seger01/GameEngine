#include "Camera.h"

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

Vector2 Camera::getOrigin()
{
	return getTransform().position - Vector2(static_cast<int>(getWidth() / 2.0f), static_cast<int>(getHeight() / 2.0f));
}

void Camera::setViewport(const FRect& viewport) { mViewport = viewport; }

FRect Camera::getViewport() const { return mViewport; }
