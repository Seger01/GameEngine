/**
 * @file Camera.h
 *
 * Contains definition of the camera class
 */
#pragma once

#include "GameObject/GameObject.h"
#include "Global/Color.h"
#include "Structs/CameraDebugOverlay.h"
#include "Structs/FRect.h"

/**
 * @class Camera
 * @brief Camera class, represents a view in a scene.
 */
class Camera : public GameObject
{
public:
	Camera();

	void setBackgroundColor(const Color& aColor);
	Color getBackgroundColor() const;

	void setWidth(int aWidth);
	int getWidth() const;

	void setHeight(int aHeight);
	int getHeight() const;

	Vector2 getOrigin() const;

	void setViewport(const FRect& viewport);
	FRect getViewport() const;

	void setDebugOverlay(const CameraDebugOverlay& debugOverlay);
	CameraDebugOverlay getDebugOverlay() const;
	CameraDebugOverlay& getDebugOverlayRef();

	void setRenderOrder(uint aRenderOrder);
	uint getRenderOrder() const;

	bool isMainCamera() const;
	void setMainCamera(bool aIsMainCamera);

	void update();

	void startShake(float duration, float magnitude);
	void stopShake();

private:
	bool mIsMainCamera; // Whether this camera is the main camera
	uint mRenderOrder;	// position in the camera render queue

	/// @brief Background color, used to render every place where there is no renderable object.
	Color mBackgroundColor;
	/// @brief Width of the camera, in game units.
	int mWidth;
	/// @brief  Height of the camera, in game units.
	int mHeight;
	/// @brief Portion of the screen this camera renders to
	FRect mViewport;

	bool mIsShaking;	   // Whether the camera is currently shaking
	float mShakeStartTime; // When the shake started
	float mShakeDuration;  // How long the shake will last
	float mShakeMagnitude; // The intensity of the shake
	Vector2 mShakeOffset;  // The current offset of the camera due to the shake

	CameraDebugOverlay mDebugOverlay;
};
