#pragma once

#include <vector>

#include "Component/Component.h"
#include "Structs/CollisionFilters.h"
#include "Structs/Transform.h"

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
	virtual ~Collider();

	virtual std::unique_ptr<Component> clone() const override;

	// Getter and setter methods for transform
	Transform getTransform() const;
	void setTransform(const Transform& transform);

private:
	Transform mTransform;
};
