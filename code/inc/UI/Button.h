/**
 * @file Button.h
 * @brief Defines the Button class, responsible for managing and rendering buttons in the game.
 */

#ifndef BUTTON_H
#define BUTTON_H

#include "Structs/Vector2.h"
#include "UI/UIObject.h"
#include <functional> // For std::function

/**
 * @struct BoundingBox
 * @brief Represents a bounding box in 2D space.
 */
struct BoundingBox
{
	Vector2 topLeft;
	Vector2 bottomRight;

	bool contains(Vector2 aPoint) const
	{
		return aPoint.x >= topLeft.x && aPoint.x <= bottomRight.x && aPoint.y >= topLeft.y && aPoint.y <= bottomRight.y;
	}
};

/**
 * @class Button
 * @brief Represents a button in the game.
 */
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
	/// @brief The width of the button
	float mWidth;
	/// @brief The height of the button
	float mHeight;
	/// @brief Whether the button is interactable
	bool mInteractable;

	/// @brief Whether the button is hovered
	bool mHovered;

	/// @brief The callback to be called when the button is clicked
	std::function<void()> mOnClickCallback;
	/// @brief The callback to be called when the button is released
	std::function<void()> mOnReleaseCallback;
};

#endif // BUTTON_H
