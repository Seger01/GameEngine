#ifndef BUTTON_H
#define BUTTON_H

#include "UIObject.h"
#include "Vector2.h"
#include <functional> // For std::function

struct BoundingBox
{
	Vector2 topLeft;
	Vector2 bottomRight;

	bool contains(Vector2 aPoint) const
	{
		return aPoint.x >= topLeft.x && aPoint.x <= bottomRight.x && aPoint.y >= topLeft.y && aPoint.y <= bottomRight.y;
	}
};

class Button : public UIObject
{
public:
	Button();

	// Set callback for button click
	void setOnClickCallback(const std::function<void()>& callback);
	void activateOnClickCallback();

	// Set callback for button click
	void setOnReleaseCallback(const std::function<void()>& callback);
	void activateOnReleaseCallback();

	bool isHovered() const;
	void setHovered(bool aHovered);

	// Width and height management
	void setWidth(float aWidth);
	float getWidth() const;

	void setHeight(float aHeight);
	float getHeight() const;

	// Interactable state management
	void setInteractable(bool aInteractable);
	bool interactable() const;

	// Get the button's bounding box
	BoundingBox getBoundingBox() const;

private:
	float mWidth;
	float mHeight;
	bool mInteractable;

	bool mHovered;

	// Callback function for the button
	std::function<void()> mOnClickCallback;
	std::function<void()> mOnReleaseCallback;
};

#endif // BUTTON_H
