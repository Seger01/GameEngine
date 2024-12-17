#include "Text.h"

Text::Text() : UIObject(), mText(""), mFont(""), mColor(Color(0, 0, 0)), mScale(Vector2(1, 1)), mLayer(0) {}

Text::Text(const std::string& aText, const std::string& aFont, const Color& aColor, const Vector2 aLocation,
		   const Vector2& aScale)
	: UIObject(), mText(aText), mFont(aFont), mColor(aColor), mScale(aScale), mLayer(0)
{
	mTransform.position = aLocation;
}

std::string Text::getText() const { return mText; }

void Text::setText(const std::string& aText) { mText = aText; }

void Text::setFont(const std::string& aFont) { mFont = aFont; }

std::string Text::getFont() const { return mFont; }

Color Text::getColor() const { return mColor; }

void Text::setColor(const Color& aColor) { mColor = aColor; }

Vector2 Text::getScale() const { return mScale; }

void Text::setScale(const Vector2& aScale) { mScale = aScale; }

int Text::getLayer() const { return mLayer; }

void Text::setLayer(int aLayer) { mLayer = aLayer; }
