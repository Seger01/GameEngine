/**
 * @file Vector2.h
 * @brief Defines the Vector2 struct.
 */

#ifndef VECTOR2_H
#define VECTOR2_H

/**
 * @struct Vector2
 * @brief Defines a 2D vector.
 */
struct Vector2
{
	float x;
	float y;

	Vector2() : x(0), y(0) {}

	Vector2(float aX, float aY) : x(aX), y(aY) {}

	// Overload the + operator for Vector2
	Vector2 operator+(const Vector2& other) const { return Vector2(x + other.x, y + other.y); }

	// Overload the - operator for Vector2
	Vector2 operator-(const Vector2& other) const { return Vector2(x - other.x, y - other.y); }

	// Overload the * operator for Vector2
	Vector2 operator*(float scalar) const { return Vector2(x * scalar, y * scalar); }

	// Overload the * operator for component-wise multiplication
	Vector2 operator*(const Vector2& other) const { return Vector2(x * other.x, y * other.y); }

	// Overload the / operator for Vector2
	Vector2 operator/(float scalar) const { return Vector2(x / scalar, y / scalar); }

	// Overload the == operator for Vector2
	bool operator==(const Vector2& other) const { return x == other.x && y == other.y; }

	// Overload the != operator for Vector2
	bool operator!=(const Vector2& other) const { return x != other.x || y != other.y; }

	// Overload the += operator for Vector2
	Vector2& operator+=(const Vector2& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	// Overload the -= operator for Vector2
	Vector2& operator-=(const Vector2& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	// Overload the *= operator for Vector2
	Vector2& operator*=(float scalar)
	{
		x *= scalar;
		y *= scalar;
		return *this;
	}

	// Overload the = operator for Vector2
	Vector2& operator=(const Vector2& other)
	{
		x = other.x;
		y = other.y;
		return *this;
	}
};

#endif
