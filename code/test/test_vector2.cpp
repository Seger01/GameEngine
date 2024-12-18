#include "Vector2.h"
#include <gtest/gtest.h>

// Test case for Vector2 initialization
TEST(Vector2Test, Initialization)
{
	// Test default constructor
	Vector2 defaultVector;
	EXPECT_EQ(defaultVector.x, 0);
	EXPECT_EQ(defaultVector.y, 0);

	// Test parameterized constructor
	Vector2 paramVector(3.5f, -7.2f);
	EXPECT_EQ(paramVector.x, 3.5f);
	EXPECT_EQ(paramVector.y, -7.2f);
}

// Test case for Vector2 arithmetic operators
TEST(Vector2Test, ArithmeticOperators)
{
	Vector2 v1(1, 2);
	Vector2 v2(3, 4);

	// Test + operator
	Vector2 sum = v1 + v2;
	EXPECT_EQ(sum.x, 4);
	EXPECT_EQ(sum.y, 6);

	// Test - operator
	Vector2 diff = v1 - v2;
	EXPECT_EQ(diff.x, -2);
	EXPECT_EQ(diff.y, -2);

	// Test scalar * operator
	Vector2 scaled = v1 * 2.0f;
	EXPECT_EQ(scaled.x, 2);
	EXPECT_EQ(scaled.y, 4);

	// Test component-wise * operator
	Vector2 componentMult = v1 * v2;
	EXPECT_EQ(componentMult.x, 3);
	EXPECT_EQ(componentMult.y, 8);

	// Test scalar / operator
	Vector2 divided = v2 / 2.0f;
	EXPECT_EQ(divided.x, 1.5f);
	EXPECT_EQ(divided.y, 2.0f);
}

// Test case for Vector2 comparison operators
TEST(Vector2Test, ComparisonOperators)
{
	Vector2 v1(1, 2);
	Vector2 v2(1, 2);
	Vector2 v3(2, 3);

	// Test == operator
	EXPECT_TRUE(v1 == v2);
	EXPECT_FALSE(v1 == v3);

	// Test != operator
	EXPECT_FALSE(v1 != v2);
	EXPECT_TRUE(v1 != v3);
}

// Test case for Vector2 compound assignment operators
TEST(Vector2Test, CompoundAssignmentOperators)
{
	Vector2 v1(1, 2);
	Vector2 v2(3, 4);

	// Test += operator
	v1 += v2;
	EXPECT_EQ(v1.x, 4);
	EXPECT_EQ(v1.y, 6);

	// Test -= operator
	v1 -= v2;
	EXPECT_EQ(v1.x, 1);
	EXPECT_EQ(v1.y, 2);

	// Test *= operator
	v1 *= 2.0f;
	EXPECT_EQ(v1.x, 2);
	EXPECT_EQ(v1.y, 4);
}

// Test case for Vector2 assignment operator
TEST(Vector2Test, AssignmentOperator)
{
	Vector2 v1(1, 2);
	Vector2 v2;

	v2 = v1;
	EXPECT_EQ(v2.x, 1);
	EXPECT_EQ(v2.y, 2);

	// Modify v1 to ensure v2 is a copy, not a reference
	v1.x = 10;
	EXPECT_EQ(v2.x, 1); // v2 should remain unchanged
}
