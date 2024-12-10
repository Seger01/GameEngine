#ifndef COMPONENT_H
#define COMPONENT_H

#include <memory>
#include <string>

class GameObject;

class Component
{
public:
	Component(std::string aTag = "defaultComponent");
	virtual ~Component() = default;

	// Rule of Five
	Component(const Component& other);				  // Copy constructor
	Component& operator=(const Component& other);	  // Copy assignment operator
	Component(Component&& other) noexcept;			  // Move constructor
	Component& operator=(Component&& other) noexcept; // Move assignment operator

	// Virtual factory method
	virtual std::unique_ptr<Component> clone() const = 0;

	void setGameObjectParent(GameObject* aParentObject) { mGameObject = aParentObject; }

	bool isActive() { return mActive; }

	void setActive(bool aState) { mActive = aState; }

	std::string getTag() { return mTag; }

	void setTag(const std::string& aTag) { mTag = aTag; }

protected:
	GameObject* mGameObject = nullptr;

	std::string mTag = "";
	bool mActive;
};

#endif
