/**
 * @file Camera.cpp
 * @brief Implementation of the camera class
 */
#include "Camera.h"

#include "Time.h"

/**
 * @brief Constructor. Sets the width and height to 800*600 by default.
 */
Camera::Camera()
	: GameObject(), mBackgroundColor(Color(0, 0, 0)), mWidth(800), mHeight(600), mViewport(FRect{0, 0, 1, 1}),
	  mDebugOverlay(CameraDebugOverlay()), mRenderOrder(0), mIsMainCamera(true), mIsShaking(false),
	  mShakeOffset(Vector2(0, 0)), mShakeStartTime(0), mShakeDuration(0), mShakeMagnitude(0)
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
	return getTransform().position - offset + mShakeOffset;
}

void Camera::setViewport(const FRect& viewport) { mViewport = viewport; }

FRect Camera::getViewport() const { return mViewport; }

void Camera::setDebugOverlay(const CameraDebugOverlay& debugOverlay) { mDebugOverlay = debugOverlay; }

CameraDebugOverlay Camera::getDebugOverlay() const { return mDebugOverlay; }

CameraDebugOverlay& Camera::getDebugOverlayRef() { return mDebugOverlay; }

void Camera::setRenderOrder(uint aRenderOrder) { mRenderOrder = aRenderOrder; }

uint Camera::getRenderOrder() const { return mRenderOrder; }

bool Camera::isMainCamera() const { return mIsMainCamera; }

void Camera::setMainCamera(bool aIsMainCamera) { mIsMainCamera = aIsMainCamera; }

void Camera::update()
{
	Vector2 shakeOffset(0, 0);
	if (mIsShaking)
	{
		float shakeTimeElapsed = Time::ticks - mShakeStartTime;

		// Reduce the shake magnitude over time (optional easing function)
		float progress = shakeTimeElapsed / mShakeDuration;
		if (progress >= 1.0f)
		{
			stopShake();
			return;
		}

		// Apply random shake offsets (using the magnitude)
		shakeOffset.x = (rand() % 2 == 0 ? 1 : -1) * mShakeMagnitude * (1.0f - progress);
		shakeOffset.y = (rand() % 2 == 0 ? 1 : -1) * mShakeMagnitude * (1.0f - progress);
	}
	else
	{
		shakeOffset = Vector2(0, 0);
	}
	mShakeOffset = shakeOffset;
	return;
}

void Camera::startShake(float duration, float magnitude)
{
	mShakeDuration = duration;
	mShakeMagnitude = magnitude;
	mShakeStartTime = Time::ticks;
	mIsShaking = true;
}

void Camera::stopShake()
{
	mIsShaking = false;
	mShakeOffset = Vector2(0, 0);
	mShakeStartTime = 0;
	mShakeDuration = 0;
	mShakeMagnitude = 0;
}
