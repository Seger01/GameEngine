#include "Button.h"
#include <gtest/gtest.h>

// Test initialization of Button
TEST(ButtonTest, Initialization) {
    Button button;
    EXPECT_EQ(button.getWidth(), 0.0f);
    EXPECT_EQ(button.getHeight(), 0.0f);
    EXPECT_TRUE(button.interactable());
    EXPECT_FALSE(button.isHovered());
}

// Test width and height setters and getters
TEST(ButtonTest, WidthHeightManagement) {
    Button button;
    button.setWidth(150.0f);
    button.setHeight(75.0f);

    EXPECT_EQ(button.getWidth(), 150.0f);
    EXPECT_EQ(button.getHeight(), 75.0f);
}

// Test interactable state management
TEST(ButtonTest, InteractableManagement) {
    Button button;
    EXPECT_TRUE(button.interactable());

    button.setInteractable(false);
    EXPECT_FALSE(button.interactable());
}

// Test hovered state management
TEST(ButtonTest, HoveredStateManagement) {
    Button button;
    EXPECT_FALSE(button.isHovered());

    button.setHovered(true);
    EXPECT_TRUE(button.isHovered());
}

// Test callback functionality for onClick
TEST(ButtonTest, OnClickCallback) {
    Button button;
    bool wasClicked = false;

    button.setOnClickCallback([&]() { wasClicked = true; });

    EXPECT_FALSE(wasClicked);
    button.activateOnClickCallback();
    EXPECT_TRUE(wasClicked);
}

// Test callback functionality for onRelease
TEST(ButtonTest, OnReleaseCallback) {
    Button button;
    bool wasReleased = false;

    button.setOnReleaseCallback([&]() { wasReleased = true; });

    EXPECT_FALSE(wasReleased);
    button.activateOnReleaseCallback();
    EXPECT_TRUE(wasReleased);
}

// Test bounding box computation
TEST(ButtonTest, BoundingBoxComputation) {
    Button button;
    button.setWidth(100.0f);
    button.setHeight(50.0f);

    button.setTransform(Transform(Vector2(10.0f, 20.0f)));
    BoundingBox box = button.getBoundingBox();

    EXPECT_EQ(box.topLeft.x, 10.0f);
    EXPECT_EQ(box.topLeft.y, 20.0f);
    EXPECT_EQ(box.bottomRight.x, 110.0f);
    EXPECT_EQ(box.bottomRight.y, 70.0f);
}

// Test bounding box contains point
TEST(ButtonTest, BoundingBoxContainsPoint) {
    Button button;
    button.setWidth(100.0f);
    button.setHeight(50.0f);

    button.setTransform(Transform(Vector2(10.0f, 20.0f)));
    BoundingBox box = button.getBoundingBox();

    EXPECT_TRUE(box.contains(Vector2(15.0f, 25.0f)));   // Inside
    EXPECT_FALSE(box.contains(Vector2(150.0f, 25.0f))); // Outside
}
