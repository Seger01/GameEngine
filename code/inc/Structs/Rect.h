/**
 * @file Rect.h
 * @brief Defines the Rect struct.
 */
#pragma once

/**
 * @struct Rect
 * @brief Defines a rectangle.
 */
struct Rect
{
	int x = 0, y = 0;
	int w = 0, h = 0;

	// Function to check if this triangle intersects with another triangle
	bool intersects(const Rect& other) const
	{
		return x < other.x + other.w && x + w > other.x && y < other.y + other.h && y + h > other.y;
	}

	// overload the == operator for Rect
	bool operator==(const Rect& other) const { return x == other.x && y == other.y && w == other.w && h == other.h; }
};
