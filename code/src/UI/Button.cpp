/**
 * @file Button.cpp
 * @brief Defines the Button class, responsible for managing and rendering buttons in the game.
 */

#include "UI/Button.h"

/**
 * @brief Default constructor for the Button class.
 */

Button::Button() : UIObject(), mWidth(0), mHeight(0), mInteractable(true), mOnClickCallback(nullptr), mHovered(false) {}

/**
 * @brief Set the callback for the button click.
 *
 * @param callback The callback function to set.
 */
void Button::setOnClickCallback(const std::function<void()>& callback) { mOnClickCallback = callback; }

/**
 * @brief Activate the callback for the button click.
 */
void Button::activateOnClickCallback()
{
	if (mOnClickCallback && mIsActive)
		mOnClickCallback();
}

/**
 * @brief Set the callback for the button release.
 *
 * @param callback The callback function to set.
 */
void Button::setOnReleaseCallback(const std::function<void()>& callback) { mOnReleaseCallback = callback; }

/**
 * @brief Activate the callback for the button release.
 */
void Button::activateOnReleaseCallback()
{
	if (mOnReleaseCallback && mIsActive)
		mOnReleaseCallback();
}

/**
 * @brief Check if the button is hovered.
 *
 * @return True if the button is hovered, false otherwise.
 */
bool Button::isHovered() const { return mHovered; }

/**
 * @brief Set the hovered state of the button.
 *
 * @param aHovered The hovered state to set.
 */
void Button::setHovered(bool aHovered) { mHovered = aHovered; }

/**
 * @brief Set the width of the button.
 *
 * @param aWidth The width to set.
 */
void Button::setWidth(float aWidth) { mWidth = aWidth; }

/**
 * @brief Set the height of the button.
 *
 * @param aHeight The height to set.
 */
void Button::setHeight(float aHeight) { mHeight = aHeight; }

/**
 * @brief Get the width of the button.
 *
 * @return The width of the button.
 */
float Button::getWidth() const { return mWidth; }

/**
 * @brief Get the height of the button.
 *
 * @return The height of the button.
 */
float Button::getHeight() const { return mHeight; }

/**
 * @brief Set the interactable state of the button.
 *
 * @param aInteractable The interactable state to set.
 */
void Button::setInteractable(bool aInteractable) { mInteractable = aInteractable; }

/**
 * @brief Get the interactable state of the button.
 *
 * @return The interactable state of the button.
 */
bool Button::interactable() const { return mInteractable; }

/**
 * @brief Get the bounding box of the button.
 *
 * @return The bounding box of the button.
 */
BoundingBox Button::getBoundingBox() const
{
	BoundingBox box;
	box.topLeft = mTransform.position;
	box.bottomRight = mTransform.position + Vector2(mWidth, mHeight);
	return box;
}
