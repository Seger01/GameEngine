/**
 * @file Camera.h
 *
 * Contains definition of the camera class
 */
#pragma once

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

private:
	/// @brief Background color, used to render every place where there is no renderable object.
	Color mBackgroundColor;
	/// @brief Width of the camera, in game units.
	int mWidth;
	int mHeight;
	FRect mViewport; // Portion of the screen this camera renders to
};
