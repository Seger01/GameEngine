/**
 * @file Camera.h
 *
 * Contains definition of the camera class
 */
#pragma once

#include "CameraDebugOverlay.h"
#include "Color.h"
#include "FRect.h"
#include "GameObject.h"

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

private:
	uint mRenderOrder; // position in the camera render queue

	/// @brief Background color, used to render every place where there is no renderable object.
	Color mBackgroundColor;
	/// @brief Width of the camera, in game units.
	int mWidth;
	/// @brief  Height of the camera, in game units.
	int mHeight;
	/// @brief Portion of the screen this camera renders to
	FRect mViewport;

	CameraDebugOverlay mDebugOverlay;
};
