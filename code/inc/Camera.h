#ifndef CAMERA_H
#define CAMERA_H

#include "Color.h"
#include "FRect.h"
#include "GameObject.h"

class Camera : public GameObject
{
public:
	Camera();
	virtual ~Camera() = default;

	void setBackgroundColor(Color aColor);
	Color getBackgroundColor();

	void setWidth(int aWidth);
	int getWidth();

	void setHeight(int aHeight);
	int getHeight();

	Vector2 getOrigin();

	void setViewport(const FRect& viewport);
	FRect getViewport() const;

private:
	Color mBackgroundColor;
	int mWidth;
	int mHeight;
	FRect mViewport; // Portion of the screen this camera renders to
};

#endif // CAMERA_H
