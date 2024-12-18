#include "Transform.h"
#include <gtest/gtest.h>

// Test case for Transform initialization
TEST(TransformTest, Initialization)
{
	// Test default constructor
	Transform defaultTransform;
	EXPECT_EQ(defaultTransform.position, Vector2(0, 0));
	EXPECT_EQ(defaultTransform.rotation, 0);
	EXPECT_EQ(defaultTransform.scale, Vector2(1, 1));

	// Test constructor with position
	Transform positionOnlyTransform(Vector2(5, 10));
	EXPECT_EQ(positionOnlyTransform.position, Vector2(5, 10));
	EXPECT_EQ(positionOnlyTransform.rotation, 0);
	EXPECT_EQ(positionOnlyTransform.scale, Vector2(1, 1));

	// Test constructor with all parameters
	Transform fullTransform(Vector2(5, 10), 45, Vector2(2, 3));
	EXPECT_EQ(fullTransform.position, Vector2(5, 10));
	EXPECT_EQ(fullTransform.rotation, 45);
	EXPECT_EQ(fullTransform.scale, Vector2(2, 3));
}

// Test case for Transform operator overloading
TEST(TransformTest, Operators)
{
	Transform t1(Vector2(1, 2), 10, Vector2(2, 2));
	Transform t2(Vector2(3, 4), 20, Vector2(1, 1));

	// Test + operator
	Transform t3 = t1 + t2;
	EXPECT_EQ(t3.position, Vector2(4, 6));
	EXPECT_EQ(t3.rotation, 30);
	EXPECT_EQ(t3.scale, Vector2(3, 3));

	// Test += operator
	Transform t4 = t1;
	t4 += t2;

	// Test == and != operators
	EXPECT_FALSE(t1 == t2);
	EXPECT_TRUE(t1 != t2);
	EXPECT_TRUE(t1 == t1);
}

// Test case for Transform helper functions
TEST(TransformTest, HelperFunctions)
{
	Transform t(Vector2(1, 2), 10, Vector2(2, 2));

	// Test translate
	t.translate(Vector2(3, 4));
	EXPECT_EQ(t.position, Vector2(4, 6));

	// Test rotate
	t.rotate(15);
	EXPECT_EQ(t.rotation, 25);

	// Test scaleBy
	t.scaleBy(Vector2(0.5, 2));
	EXPECT_EQ(t.scale, Vector2(1, 4));
}
