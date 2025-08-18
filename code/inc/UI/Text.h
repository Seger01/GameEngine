/**
 * @file Text.h
 * @brief Defines the Text class, responsible for managing and rendering text in the game.
 */

#ifndef TEXT_H
#define TEXT_H

#include <string>

#include "Global/Color.h"
#include "Structs/Vector2.h"
#include "UI/UIObject.h"

/**
 * @class Text
 * @brief Represents text in the game.
 */
class Text : public UIObject
{
public:
	Text();
	Text(const std::string& aText, const std::string& aFont, const Color& aColor, const Vector2 aLocation,
		 const Vector2& aScale);

	std::string getText() const;
	void setText(const std::string& aText);

	std::string getFont() const;
	void setFont(const std::string& aFont);

	Color getColor() const;
	void setColor(const Color& aColor);

	Vector2 getScale() const;
	void setScale(const Vector2& aScale);

	int getLayer() const;
	void setLayer(int aLayer);

private:
	/// @brief The layer of the text
	int mLayer;

	/// @brief The text to display
	std::string mText;
	/// @brief The font of the text
	std::string mFont;
	/// @brief The color of the text
	Color mColor;
	/// @brief The location of the text
	Vector2 mScale;
};

#endif // TEXT_H
