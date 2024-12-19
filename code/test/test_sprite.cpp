#include "Components/Sprite.h"
#include <gtest/gtest.h>

// Test case for Sprite initialization
TEST(SpriteTest, Initialization)
{
	Texture texture(nullptr, 0);
	Rect sourceRect{0, 0, 32, 32};
	std::string tag = "player";

	Sprite sprite(texture, 32, 32, sourceRect, tag);

	EXPECT_EQ(sprite.getTexture().getSDLTexture(), texture.getSDLTexture());
	EXPECT_EQ(sprite.getSource(), sourceRect);
	EXPECT_EQ(sprite.getColorFilter(), Color(255, 255, 255, 255)); // Default Color
	EXPECT_EQ(sprite.getRelativePosition(), Transform());		   // Default Transform
}

// Test case for getter and setter methods
TEST(SpriteTest, GetterAndSetters)
{
	Texture texture(nullptr, 0);
	Rect sourceRect{0, 0, 32, 32};
	std::string tag = "player";

	Sprite sprite(texture, 32, 32, sourceRect, tag);

	// Test setSource and getSource
	Rect newSource{10, 10, 64, 64};
	sprite.setSource(newSource);
	EXPECT_EQ(sprite.getSource(), newSource);

	// Test setColorFilter and getColorFilter
	Color newColor(100, 150, 200, 255);
	sprite.setColorFilter(newColor);
	EXPECT_EQ(sprite.getColorFilter(), newColor);

	// Test setRelativePosition and getRelativePosition
	Transform newTransform(Vector2(10, 20), 45, Vector2(2, 2));
	sprite.setRelativePosition(newTransform);
	EXPECT_EQ(sprite.getRelativePosition(), newTransform);
}

// Test case for clone method
TEST(SpriteTest, Clone)
{
	Texture texture(nullptr, 0);
	Rect sourceRect{0, 0, 32, 32};
	std::string tag = "player";

	Sprite sprite(texture, 32, 32, sourceRect, tag);

	auto clonedSprite = sprite.clone();
	auto clonedSpritePtr = dynamic_cast<Sprite*>(clonedSprite.get());
	ASSERT_NE(clonedSpritePtr, nullptr);

	// Check that the cloned sprite matches the original
	EXPECT_EQ(clonedSpritePtr->getTexture().getSDLTexture(), sprite.getTexture().getSDLTexture());
	EXPECT_EQ(clonedSpritePtr->getSource(), sprite.getSource());
	EXPECT_EQ(clonedSpritePtr->getColorFilter(), sprite.getColorFilter());
	EXPECT_EQ(clonedSpritePtr->getRelativePosition(), sprite.getRelativePosition());
}

// Test case for move assignment and move constructor
TEST(SpriteTest, MoveSemantics)
{
	Texture texture(nullptr, 0);
	Rect sourceRect{0, 0, 32, 32};
	std::string tag = "player";

	Sprite original(texture, 32, 32, sourceRect, tag);

	// Test move constructor
	Sprite movedSprite(std::move(original));
	EXPECT_EQ(movedSprite.getTexture().getSDLTexture(), texture.getSDLTexture());
	EXPECT_EQ(movedSprite.getSource(), sourceRect);

	// Test move assignment
	Sprite anotherSprite(texture, 16, 16, sourceRect, "enemy");
	anotherSprite = std::move(movedSprite);
	EXPECT_EQ(anotherSprite.getTexture().getSDLTexture(), texture.getSDLTexture());
	EXPECT_EQ(anotherSprite.getSource(), sourceRect);
}
