#include "Camera.h"

Camera::Camera() : mBackgroundColor(Color(0, 0, 0)), mWidth(800), mHeight(600) {}

Camera::~Camera() {}

void Camera::setBackgroundColor(Color aColor) { mBackgroundColor = aColor; }

Color Camera::getBackgroundColor() { return mBackgroundColor; }

void Camera::setWidth(int aWidth) { mWidth = aWidth; }

int Camera::getWidth() { return mWidth; }

void Camera::setHeight(int aHeight) { mHeight = aHeight; }

int Camera::getHeight() { return mHeight; }
