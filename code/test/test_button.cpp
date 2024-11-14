#include "Button.h"
#include <gtest/gtest.h>
#include <iostream>
#include <sstream>

class ButtonTest : public ::testing::Test {
protected:
    Button* button;

    void SetUp() override { button = new Button(); }

    void TearDown() override { delete button; }
};

TEST_F(ButtonTest, DefaultValues) {
    // Verify the default values for width, height, and interactable
    EXPECT_EQ(button->getWidth(), 0);
    EXPECT_EQ(button->getHeight(), 0);
    EXPECT_TRUE(button->interactable());
}

TEST_F(ButtonTest, SetWidth) {
    button->setWidth(150.0f);
    EXPECT_EQ(button->getWidth(), 150.0f);
}

TEST_F(ButtonTest, SetHeight) {
    button->setHeight(75.0f);
    EXPECT_EQ(button->getHeight(), 75.0f);
}

TEST_F(ButtonTest, SetInteractable) {
    button->setInteractable(false);
    EXPECT_FALSE(button->interactable());

    button->setInteractable(true);
    EXPECT_TRUE(button->interactable());
}

TEST_F(ButtonTest, OnClick) {
    std::ostringstream output;
    std::streambuf* oldCout = std::cout.rdbuf(output.rdbuf()); // Redirect cout to capture output

    button->OnClick();

    std::cout.rdbuf(oldCout); // Restore cout

    EXPECT_EQ(output.str(), "Button Clicked!\n");
}

TEST_F(ButtonTest, BoundingBox) {
    // Set position and size of button
    button->setWidth(100.0f);
    button->setHeight(50.0f);
    button->setTransform(Transform(Vector2(10, 20)));

    // Get bounding box and verify values
    BoundingBox box = button->getBoundingBox();
    EXPECT_EQ(box.topLeft, Vector2(10, 20));
    EXPECT_EQ(box.bottomRight, Vector2(110, 70));
}

TEST_F(ButtonTest, BoundingBox_DefaultValues) {
    // Verify that the default bounding box is zero-sized at the origin
    BoundingBox box = button->getBoundingBox();
    EXPECT_EQ(box.topLeft, Vector2(0, 0));
    EXPECT_EQ(box.bottomRight, Vector2(0, 0));
}
