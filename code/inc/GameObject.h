/**
 * @file GameObject.h
 *
 * @brief Contains the defintionn of the GameObject class.
 */
#pragma once

#include "Component.h"
#include "Transform.h"
#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <vector>

/**
 * @class GameObject
 *
 * @brief Represents a game object in the game world. Is the base of everything that should be seen or heard in the
 * game.
 */
class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	// Rule of Five
	GameObject(const GameObject& other);				// Copy constructor
	GameObject& operator=(const GameObject& other);		// Copy assignment operator
	GameObject(GameObject&& other) noexcept;			// Move constructor
	GameObject& operator=(GameObject&& other) noexcept; // Move assignment operator

	void addComponent(Component* aComponent);
	void removeComponent(Component* component);

	void setID(int id);
	int getID() const;

	void setName(const std::string& name);
	std::string getName() const;

	void setTag(const std::string& tag);
	std::string getTag() const;

	void setActive(bool isActive);
	bool isActive() const;

	Transform getTransform() const;
	Transform& getTransformRef();
	void setTransform(Transform aNewTransform);

	void setParent(GameObject& parent);
	void removeParent();
	GameObject& getParent();

	void addChild(GameObject& child);
	void removeChild(GameObject& child);
	std::vector<std::reference_wrapper<GameObject>> getChildren() const;

	std::vector<std::reference_wrapper<Component>> getComponentsWithTag(const std::string& tag) const;

	// Templated functions
	/**
	 * @brief Checks if the GameObject has a component of type T.
	 *
	 * @tparam T The type of the component to check for.
	 *
	 * @return True if the GameObject has a component of type T, false otherwise.
	 */
	template <typename T> bool hasComponent() const
	{
		for (const std::unique_ptr<Component>& component : mComponents)
		{
			if (dynamic_cast<T*>(component.get()) != nullptr)
			{
				return true;
			}
		}
		return false;
	}

	/**
	 * @brief Gets the components of type T from the GameObject.
	 *
	 * @tparam T The type of the component to get.
	 *
	 * @return A vector of references to the components of type T.
	 */
	template <typename T> std::vector<std::reference_wrapper<T>> getComponents() const
	{
		std::vector<std::reference_wrapper<T>> componentsOfType;
		for (const std::unique_ptr<Component>& component : mComponents)
		{
			if (T* casted = dynamic_cast<T*>(component.get()))
			{
				componentsOfType.push_back(*casted);
			}
		}
		return componentsOfType;
	}

	/**
	 * @brief Gets all components with a specific tag.
	 *
	 * @tparam T The type of the component to get.
	 * @param tag The tag to search for.
	 *
	 * @return A vector of references to the components with the specified tag.
	 */
	template <typename T> std::vector<std::reference_wrapper<T>> getComponentsWithTag(const std::string& tag) const
	{
		std::vector<std::reference_wrapper<T>> componentsWithTag;
		for (const std::unique_ptr<Component>& component : mComponents)
		{
			if (component->getTag() == tag)
			{
				if (T* castedComponent = dynamic_cast<T*>(component.get()))
				{
					componentsWithTag.push_back(*castedComponent);
				}
			}
		}
		return componentsWithTag;
	}

	/**
	 * @brief Adds a component of type T to the GameObject.
	 *
	 * @tparam T The type of the component to add.
	 * @param args The arguments to pass to the constructor of the component.
	 *
	 * @return A reference to the added component.
	 */
	template <typename T, typename... Args> T& addComponent(Args&&... args)
	{
		T* rawPtr = new T(std::forward<Args>(args)...);
		addComponent(rawPtr);
		return *rawPtr;
	}

protected:
	/// @brief The parent of the GameObject. Nullptr if there is no parent. Not posessive.
	GameObject* mParent;
	/// @brief The children of the GameObject.
	std::vector<std::reference_wrapper<GameObject>> mChildren;

	/// @brief The components of the GameObject. Possessive.
	std::vector<std::unique_ptr<Component>> mComponents;
	/// @brief The transform of the GameObject. Is not part of the mComponents vector, because it is a mandatory
	/// component.
	Transform mTransform;

	/// @brief The ID of the GameObject.
	int mID;
	/// @brief The name of the GameObject.
	std::string mName;
	/// @brief The tag of the GameObject.
	std::string mTag;
	/// @brief The active state of the GameObject. When the GameObject is not active, it is not used in any way.
	bool mIsActive;
};