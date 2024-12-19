#pragma once

#include "CollisionFilters.h"
#include "Component.h"
#include "Transform.h"
#include "vector"

/**
 * @file Collider.h
 *
 * @brief This file contains the declaration of the Collider class which is used to
 * store the properties of a collider and send it to the world
 *
 */

class Collider : public Component
{
public:
	Collider(const std::string& aTag = "defaultCollider");
	virtual std::unique_ptr<Component> clone() const override;

	Transform getTransform() const;
	void setTransform(const Transform& transform);

private:
	/// @brief Transform of the collider
	Transform mTransform;
};
