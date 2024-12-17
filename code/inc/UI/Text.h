#ifndef TEXT_H
#define TEXT_H

#include <string>

#include "Color.h"
#include "UIObject.h"
#include "Vector2.h"

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
	int mLayer;

	std::string mText;
	std::string mFont;
	Color mColor;
	Vector2 mScale;
	//??? mAlignment;
};

#endif // TEXT_H
