#ifndef CAMERA_H
#define CAMERA_H

#include "CameraDebugOverlay.h"
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

	void setDebugOverlay(const CameraDebugOverlay& debugOverlay);
	CameraDebugOverlay getDebugOverlay() const;
	CameraDebugOverlay& getDebugOverlayRef();

	void setRenderOrder(uint aRenderOrder);
	uint getRenderOrder() const;

private:
	uint mRenderOrder; // position in the camera render queue

	Color mBackgroundColor;
	int mWidth;
	int mHeight;
	FRect mViewport; // Portion of the screen this camera renders to

	CameraDebugOverlay mDebugOverlay;
};

#endif // CAMERA_H
