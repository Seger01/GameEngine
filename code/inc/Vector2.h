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

	/**
	 * @brief Default constructor for Vector2. Initializes x and y to 0.
	 */
	Vector2() : x(0), y(0) {}

	/**
	 * @brief Constructor for Vector2. Initializes x and y to the given values.
	 *
	 * @param aX The x value.
	 * @param aY The y value.
	 */
	Vector2(float aX, float aY) : x(aX), y(aY) {}

	/**
	 * @brief + operator overload for Vector2. Adds the x and y values of the two vectors.
	 */
	Vector2 operator+(const Vector2& other) const { return Vector2(x + other.x, y + other.y); }

	/**
	 * @brief - operator overload for Vector2. Subtracts the x and y values of the two vectors.
	 */
	Vector2 operator-(const Vector2& other) const { return Vector2(x - other.x, y - other.y); }

	/**
	 * @brief * operator overload for Vector2. Multiplies the x and y values of the vector by a scalar.
	 */
	Vector2 operator*(float scalar) const { return Vector2(x * scalar, y * scalar); }

	/**
	 * @brief * operator overload for Vector2. Multiplies the x and y values of the two vectors.
	 */
	Vector2 operator*(const Vector2& other) const { return Vector2(x * other.x, y * other.y); }

	/**
	 * @brief / operator overload for Vector2. Divides the x and y values of the vector by a scalar.
	 */
	Vector2 operator/(float scalar) const { return Vector2(x / scalar, y / scalar); }

	/**
	 * @brief == operator. Checks if the x and y values of the two vectors are equal.
	 */
	bool operator==(const Vector2& other) const { return x == other.x && y == other.y; }

	/**
	 * @brief != operator. Checks if the x or y values of the two vectors are not equal.
	 */
	bool operator!=(const Vector2& other) const { return x != other.x || y != other.y; }

	/**
	 * @brief += operator for Vector2. Adds the x and y values of the two vectors.
	 */
	Vector2& operator+=(const Vector2& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	/**
	 * @brief -= operator for Vector2. Subtracts the x and y values of the two vectors.
	 */
	Vector2& operator-=(const Vector2& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	/**
	 * @brief *= operator for Vector2. Multiplies the x and y values of the vector by a scalar.
	 */
	Vector2& operator*=(float scalar)
	{
		x *= scalar;
		y *= scalar;
		return *this;
	}

	/**
	 * @brief *= operator for Vector2. Multiplies the x and y values of the two vectors.
	 */
	Vector2& operator=(const Vector2& other)
	{
		x = other.x;
		y = other.y;
		return *this;
	}
};

#endif
