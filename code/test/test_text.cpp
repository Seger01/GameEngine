#include "Color.h"
#include "Text.h"
#include "Vector2.h"
#include <gtest/gtest.h>

// Helper constants for the test
const std::string defaultText = "Hello World";
const std::string defaultFont = "Arial";
const Color defaultColor{255, 255, 255, 255}; // White
const Vector2 defaultLocation{50, 100};
const Vector2 defaultScale{1.0f, 1.0f};
const int defaultLayer = 1;

TEST(TextTest, DefaultConstructor) {
    Text text;

    EXPECT_EQ(text.getText(), "");
    EXPECT_EQ(text.getFont(), "");
    EXPECT_EQ(text.getColor(), Color(0, 0, 0, 255)); // Assuming default color is black
    EXPECT_EQ(text.getScale(), Vector2(1.0f, 1.0f)); // Assuming default scale is 1.0
    EXPECT_EQ(text.getLayer(), 0);                   // Assuming default layer is 0
}

TEST(TextTest, ParameterizedConstructor) {
    Text text(defaultText, defaultFont, defaultColor, defaultLocation, defaultScale);

    EXPECT_EQ(text.getText(), defaultText);
    EXPECT_EQ(text.getFont(), defaultFont);
    EXPECT_EQ(text.getColor(), defaultColor);
    EXPECT_EQ(text.getScale(), defaultScale);
}

TEST(TextTest, SetGetText) {
    Text text;
    text.setText(defaultText);

    EXPECT_EQ(text.getText(), defaultText);
}

TEST(TextTest, SetGetFont) {
    Text text;
    text.setFont(defaultFont);

    EXPECT_EQ(text.getFont(), defaultFont);
}

TEST(TextTest, SetGetColor) {
    Text text;
    text.setColor(defaultColor);

    EXPECT_EQ(text.getColor(), defaultColor);
}

TEST(TextTest, SetGetScale) {
    Text text;
    text.setScale(defaultScale);

    EXPECT_EQ(text.getScale(), defaultScale);
}

TEST(TextTest, SetGetLayer) {
    Text text;
    text.setLayer(defaultLayer);

    EXPECT_EQ(text.getLayer(), defaultLayer);
}
