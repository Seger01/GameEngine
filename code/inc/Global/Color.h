/**
 * @file Color.h
 * @brief Color class header file.
 */

#ifndef COLOR_H
#define COLOR_H

#include <cstdint>

/**
 * @class Color
 * @brief Class for representing colors.
 */
class Color
{
public:
	Color() : r(0), g(0), b(0), a(255) {}

	Color(int r, int g, int b, int a) : r(r), g(g), b(b), a(a) {}

	Color(int r, int g, int b) : r(r), g(g), b(b), a(255) {}

	~Color() {}

	// == operator for comparing two colors
	bool operator==(const Color& other) const { return r == other.r && g == other.g && b == other.b && a == other.a; }

	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
};

#endif // COLOR_H
