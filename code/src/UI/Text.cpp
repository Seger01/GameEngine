/**
 * @file Text.cpp
 * @brief Defines the Text class, responsible for managing and rendering text in the game.
 */

#include "UI/Text.h"

/**
 * @brief Default constructor for the Text class.
 */
Text::Text() : UIObject(), mText(""), mFont(""), mColor(Color(0, 0, 0)), mScale(Vector2(1, 1)), mLayer(0) {}

/**
 * @brief Constructor for the Text class.
 * @param aText The text to display.
 * @param aFont The font of the text.
 * @param aColor The color of the text.
 * @param aLocation The location of the text.
 * @param aScale The scale of the text.
 */
Text::Text(const std::string& aText, const std::string& aFont, const Color& aColor, const Vector2 aLocation,
		   const Vector2& aScale)
	: UIObject(), mText(aText), mFont(aFont), mColor(aColor), mScale(aScale), mLayer(0)
{
	mTransform.position = aLocation;
}

/**
 * @brief Get the text of the Text object.
 * @return The text of the Text object.
 */
std::string Text::getText() const { return mText; }

/**
 * @brief Set the text of the Text object.
 * @param aText The text to set.
 */
void Text::setText(const std::string& aText) { mText = aText; }

/**
 * @brief Get the font of the Text object.
 * @return The font of the Text object.
 */
void Text::setFont(const std::string& aFont) { mFont = aFont; }

/**
 * @brief Set the font of the Text object.
 * @param aFont The font to set.
 */
std::string Text::getFont() const { return mFont; }

/**
 * @brief Get the color of the Text object.
 * @return The color of the Text object.
 */
Color Text::getColor() const { return mColor; }

/**
 * @brief Set the color of the Text object.
 * @param aColor The color to set.
 */
void Text::setColor(const Color& aColor) { mColor = aColor; }

/**
 * @brief Get the scale of the Text object.
 * @return The scale of the Text object.
 */
Vector2 Text::getScale() const { return mScale; }

/**
 * @brief Set the scale of the Text object.
 * @param aScale The scale to set.
 */
void Text::setScale(const Vector2& aScale) { mScale = aScale; }

/**
 * @brief Get the layer of the Text object.
 * @return The layer of the Text object.
 */
int Text::getLayer() const { return mLayer; }

/**
 * @brief Set the layer of the Text object.
 * @param aLayer The layer to set.
 */
void Text::setLayer(int aLayer) { mLayer = aLayer; }
