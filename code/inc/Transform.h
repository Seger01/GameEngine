/**
 * @file Transform.h
 * @brief Defines a structure to represent a 2D transformation (position, rotation, scale).
 */

#pragma once

#include "Vector2.h"

#include <cmath>

/**
 * @struct Transform
 * @brief Represents a 2D transformation (position, rotation, scale).
 */
struct Transform
{
	/// @brief Position in the world (x, y) in world coordinates
	Vector2 position;
	/// @brief Rotation angle in degrees
	float rotation;
	/// @brief Scale factor along the X and Y axes
	Vector2 scale;

	/**
	 * @brief Default constructor for Transform. Initializes position to (0, 0), rotation to 0, and scale to (1, 1).
	 */
	Transform() : position(0, 0), rotation(0), scale(1, 1) {}

	/**
	 * @brief Constructor for Transform. Initializes position to the given value, and rotation and scale to default
	 * values.
	 *
	 * @param aPosition The position of the transform.
	 */

	Transform(Vector2 aPosition) : position(aPosition), rotation(0), scale(Vector2(1, 1)) {}

	/**
	 * @brief Constructor for Transform. Initializes position, rotation, and scale to the given values.
	 *
	 * @param aPosition The position of the transform.
	 * @param aRotation The rotation of the transform.
	 * @param aScale The scale of the transform.
	 */
	Transform(Vector2 aPosition, float aRotation, Vector2 aScale)
		: position(aPosition), rotation(aRotation), scale(aScale)
	{
	}

	/**
	 * @brief += operator. Adds all individual components of the transform.
	 */
	Transform operator+=(const Transform& other) const
	{
		return Transform(position + other.position, // Sum positions (uses Vector2 +)
						 rotation + other.rotation, // Sum rotations
						 scale + other.scale		// Sum scales (uses Vector2 +)
		);
	}

	/**
	 * @brief + operator. Adds all individual components of the transform.
	 */
	Transform operator+(const Transform& other) const
	{
		return Transform(position + other.position, // Sum positions (uses Vector2 +)
						 rotation + other.rotation, // Sum rotations
						 scale + other.scale		// Sum scales (uses Vector2 +)
		);
	}

	/**
	 * @brief != operator. Checks if the position, rotation, or scale of the two transforms are not equal.
	 */
	bool operator!=(const Transform& other) const
	{
		return position != other.position || rotation != other.rotation || scale != other.scale;
	}

	/**
	 * @brief == operator. Checks if the position, rotation, and scale of the two transforms are equal.
	 */
	bool operator==(const Transform& other) const
	{
		return position == other.position && rotation == other.rotation && scale == other.scale;
	}

	/**
	 * @brief / operator. Divides all individual components of the transform by a scalar.
	 */
	Transform operator/(const int& value) { return Transform(position / value, rotation / value, scale / value); }

	/**
	 * @brief Helper function to translate the position of the transform by a given delta.
	 *
	 * @param delta The amount to translate the position by.
	 */
	void translate(const Vector2& delta)
	{
		position.x += delta.x;
		position.y += delta.y;
	}

	/**
	 * @brief Helper function to rotate the transform by a given delta. If the rotation goes outside of [0,360), it is
	 * wrapped around.
	 */
	void rotate(float deltaRotation)
	{
		rotation += deltaRotation;
		if (rotation > 360)
		{
			rotation = std::fmod(rotation, 360);
		}
		else if (rotation < 0)
		{
			// Fmod to reduce it to [-360, 0). Then add 360 to make it [0, 360)
			rotation = std::fmod(rotation, 360) + 360;
		}
	}

	/**
	 * @brief Helper function to scale the transform by a given factor.
	 *
	 * @param scaleFactor The factor to scale the transform by.
	 */
	void scaleBy(const Vector2& scaleFactor)
	{
		scale.x *= scaleFactor.x;
		scale.y *= scaleFactor.y;
	}
};
