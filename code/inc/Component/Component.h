/**
 * @file Component.h
 *
 * @brief This file contains the definition of the Component class.
 */
#pragma once

#include <memory>
#include <string>

class GameObject;

/**
 * @class Component
 *
 * @brief Represents a component that can be attached to a GameObject. This is the base class for all components, and
 * provides the basic functionality that all components should have.
 */
class Component
{
public:
	Component(const std::string& aTag = "defaultComponent");
	virtual ~Component() = default;

	virtual std::unique_ptr<Component> clone() const;

	void setGameObjectParent(GameObject* aParentObject);

	bool isActive();

	void setActive(bool aState);

	std::string getTag();

	void setTag(const std::string& aTag);

protected:
	/// @brief The GameObject that this component is attached to.
	GameObject* mGameObject;
	/// @brief The tag of the component. Can be used to identify it.
	std::string mTag;
	/// @brief Whether the component is active or not. If the component is not active, it does not in any way affect the
	/// mGameObject (it should behave as if it is not there).
	bool mActive;
};