#include "Camera.h"

Camera::Camera()
	: GameObject(), mBackgroundColor(Color(0, 0, 0, 0)), mWidth(800), mHeight(600), mViewport(FRect{0, 0, 1, 1}),
	  mDebugOverlay(CameraDebugOverlay()), mRenderOrder(0)
{
}

void Camera::setBackgroundColor(Color aColor) { mBackgroundColor = aColor; }

Color Camera::getBackgroundColor() { return mBackgroundColor; }

void Camera::setWidth(int aWidth) { mWidth = aWidth; }

int Camera::getWidth() { return mWidth; }

void Camera::setHeight(int aHeight) { mHeight = aHeight; }

int Camera::getHeight() { return mHeight; }

Vector2 Camera::getOrigin()
{
	return getTransform().position - Vector2(static_cast<int>(getWidth() / 2.0f), static_cast<int>(getHeight() / 2.0f));
}

// Implementation
void Camera::setViewport(const FRect& viewport) { mViewport = viewport; }

FRect Camera::getViewport() const { return mViewport; }

void Camera::setDebugOverlay(const CameraDebugOverlay& debugOverlay) { mDebugOverlay = debugOverlay; }

CameraDebugOverlay Camera::getDebugOverlay() const { return mDebugOverlay; }

CameraDebugOverlay& Camera::getDebugOverlayRef() { return mDebugOverlay; }

void Camera::setRenderOrder(uint aRenderOrder) { mRenderOrder = aRenderOrder; }

uint Camera::getRenderOrder() const { return mRenderOrder; }
