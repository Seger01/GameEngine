#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Component.h"
#include "Transform.h"
#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <vector>

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
	int getID();

	void setName(const std::string& name);
	std::string getName();

	void setTag(const std::string& tag);
	std::string getTag();

	void setActive(bool isActive);
	bool isActive();

	Transform getTransform() const;
	void setTransform(Transform aNewTransform);

	void setParent(GameObject* parent);
	GameObject* getParent();

	void addChild(GameObject* child);
	void removeChild(GameObject* child);
	std::vector<GameObject*> getChildren();

	std::vector<std::reference_wrapper<Component>> getComponentsWithTag(const std::string& tag) const;

	// Templated functions
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

	// Templated addComponent function
	template <typename T, typename... Args> T* addComponent(Args&&... args)
	{
		T* rawPtr = new T(std::forward<Args>(args)...);
		addComponent(rawPtr);
		return rawPtr;
	}

protected:
	GameObject* mParent;
	std::vector<GameObject*> mChildren;

	std::vector<std::unique_ptr<Component>> mComponents;
	Transform mTransform;

	int mID;
	std::string mName;
	std::string mTag;
	bool mIsActive;
};

#endif
