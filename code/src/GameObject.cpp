#include "GameObject.h"

#include <functional>
#include <iostream>
#include <memory>

#include "Component.h"
#include "EngineBravo.h"

/**
 * @brief Default constructor for GameObject.
 */
GameObject::GameObject() : mParent(nullptr), mTransform(Transform()), mID(-1), mName(""), mTag(""), mIsActive(true) {}

/**
 * @brief Destructor for GameObject. Removes this object from its parent and orphans all its children.
 */
GameObject::~GameObject()
{
	if (mParent)
	{
		mParent->removeChild(*this);
	}

	for (GameObject& child : mChildren)
	{
		child.removeParent();
	}
	mChildren.clear();

	mComponents.resize(0);
}

/**
 * @brief Copy constructor for GameObject. Deep copies all components.
 */
GameObject::GameObject(const GameObject& aOther)
	: mParent(aOther.mParent), mTransform(aOther.mTransform), mID(aOther.mID), mName(aOther.mName), mTag(aOther.mTag),
	  mIsActive(aOther.mIsActive)
{
	// Deep copy each component using its copy constructor
	for (const auto& component : aOther.mComponents)
	{
		if (component)
		{
			mComponents.push_back(std::move(component->clone())); // Using copy constructor
			mComponents.back()->setGameObjectParent(this);
		}
	}
}

/**
 * @brief Copy assignment operator for GameObject. Deep copies all components.
 *
 * @param aOther The GameObject to copy.
 *
 * @return A reference to this GameObject.
 */
GameObject& GameObject::operator=(const GameObject& aOther)
{
	if (this == &aOther)
		return *this;

	mParent = aOther.mParent;
	mTransform = aOther.mTransform;
	mID = aOther.mID;
	mName = aOther.mName;
	mTag = aOther.mTag;
	mIsActive = aOther.mIsActive;

	// Clear and deep copy each component using its copy constructor
	mComponents.clear();
	for (const auto& component : aOther.mComponents)
	{
		if (component)
		{
			mComponents.push_back(std::move(component->clone())); // Using copy constructor
		}
	}

	return *this;
}

/**
 * @brief Move constructor for GameObject.
 */
GameObject::GameObject(GameObject&& aOther) noexcept
	: mParent(aOther.mParent), mComponents(std::move(aOther.mComponents)), mTransform(std::move(aOther.mTransform)),
	  mID(aOther.mID), mName(std::move(aOther.mName)), mTag(std::move(aOther.mTag)), mIsActive(aOther.mIsActive)
{
	aOther.mParent = nullptr;
	aOther.mID = 0;
	aOther.mIsActive = false;
}

/**
 * @brief Move assignment operator for GameObject.
 *
 * @param aOther The GameObject to move.
 *
 * @return A reference to this GameObject.
 */
GameObject& GameObject::operator=(GameObject&& aOther) noexcept
{
	if (this == &aOther)
		return *this;

	mParent = aOther.mParent;
	mComponents = std::move(aOther.mComponents);
	mTransform = std::move(aOther.mTransform);
	mID = aOther.mID;
	mName = std::move(aOther.mName);
	mTag = std::move(aOther.mTag);
	mIsActive = aOther.mIsActive;

	aOther.mParent = nullptr;
	aOther.mID = 0;
	aOther.mIsActive = false;

	return *this;
}

/**
 * @brief Add a component to this GameObject. The GameObject takes ownership of the component, and sets the component's
 * parent to this GameObject.
 *
 * @param aComponent The component to add.
 *
 * @throw std::runtime_error if the component is null.
 */
void GameObject::addComponent(Component* aComponent)
{
	if (aComponent == nullptr)
	{
		throw std::runtime_error("Component is null");
	}
	aComponent->setGameObjectParent(this);
	mComponents.push_back(std::unique_ptr<Component>(aComponent));
	EngineBravo::getInstance().getUpdateQueue().addToUpdateObjects(*this);
}

/**
 * @brief Removes a component from the GameObject. If the
 */
void GameObject::removeComponent(Component* aComponent)
{
	if (aComponent == nullptr)
	{
		throw std::runtime_error("Component is null");
	}
	auto it = std::remove_if(mComponents.begin(), mComponents.end(),
							 [aComponent](const std::unique_ptr<Component>& comp) { return comp.get() == aComponent; });
	if (it != mComponents.end())
	{
		mComponents.erase(it, mComponents.end()); // unique_ptr automatically deletes the component
		EngineBravo::getInstance().getUpdateQueue().addToUpdateObjects(*this);
	}
}

/**
 * @brief Sets the GameObject's ID.
 *
 * @param aId The ID to set.
 */
void GameObject::setID(int aId) { mID = aId; }

/**
 * @brief Gets the GameObject's ID.
 *
 * @return The GameObject's ID.
 */
int GameObject::getID() const { return mID; }

/**
 * @brief Sets the GameObject's name.
 *
 * @param aName The name to set.
 */
void GameObject::setName(const std::string& aName) { mName = aName; }

/**
 * @brief Gets the GameObject's name.
 *
 * @return The GameObject's name.
 */
std::string GameObject::getName() const { return mName; }

/**
 * @brief Sets the GameObject's tag.
 *
 * @param aTag The tag to set.
 */
void GameObject::setTag(const std::string& aTag) { mTag = aTag; }

/**
 * @brief Gets the GameObject's tag.
 *
 * @return The GameObject's tag.
 */
std::string GameObject::getTag() const { return mTag; }

/**
 * @brief Sets the active state of the GameObject and all its children.
 *
 * @param aActive The active state to set.
 */
void GameObject::setActive(bool aActive)
{
	mIsActive = aActive;
	for (GameObject& child : mChildren)
	{
		child.setActive(aActive);
	}
}

/**
 * @brief Gets the active state of the GameObject.
 *
 * @return The active state of the GameObject.
 */
bool GameObject::isActive() const { return mIsActive; }

/**
 * @brief Gets the transform of the GameObject. If the GameObject has a parent, the transform is relative to the parent.
 *
 * @return The transform of the GameObject.
 */
Transform GameObject::getTransform() const
{
	if (mParent == nullptr)
	{
		return mTransform;
	}

	Transform parentTransform = mParent->getTransform();

	parentTransform.position += mTransform.position;
	parentTransform.rotation += mTransform.rotation;
	parentTransform.scale = parentTransform.scale * mTransform.scale;

	return parentTransform;
}

/**
 * @brief Gets a reference to the transform of the GameObject. If the GameObject has a parent, the transform is relative
 * to the parent.
 *
 * @return A transform of the GameObject.
 */
Transform& GameObject::getTransformRef() { return mTransform; }

/**
 * @brief Sets the transform of the GameObject.
 *
 * @param aTransform The transform to set.
 */
void GameObject::setTransform(Transform aTransform) { mTransform = aTransform; }

/**
 * @brief Sets the parent of the GameObject. Removes the GameObject from its current parent and adds it to the new
 * parent.
 *
 * @param aParent The parent to set.
 */
void GameObject::setParent(GameObject& aParent)
{
	if (mParent != nullptr)
	{
		mParent->removeChild(*this);
	}
	mParent = &aParent;
	mParent->addChild(*this);
}

/**
 * @brief Removes the parent of the GameObject.
 */
void GameObject::removeParent() { mParent = nullptr; }

/**
 * @brief Gets the parent of the GameObject.
 *
 * @return Ref to the parent of the GameObject.
 *
 * @throw std::runtime_error if the parent is null.
 */
GameObject& GameObject::getParent()
{
	if (mParent == nullptr)
	{
		throw std::runtime_error("Parent is null");
	}
	return *mParent;
}

/**
 * @brief Adds a child to the GameObject.
 *
 * @param aChild The child to add.
 */
void GameObject::addChild(GameObject& aChild) { mChildren.push_back(aChild); }

/**
 * @brief Removes a child from the GameObject.
 *
 * @param aChild The child to remove.
 */
void GameObject::removeChild(GameObject& aChild)
{
	auto it = std::remove_if(mChildren.begin(), mChildren.end(),
							 [&aChild](std::reference_wrapper<GameObject>& ref) { return &ref.get() == &aChild; });
	if (it != mChildren.end())
	{
		mChildren.erase(it, mChildren.end());
	}
}

/**
 * @brief Gets all children of the GameObject.
 *
 * @return A vector of references to the children of the GameObject.
 */
std::vector<std::reference_wrapper<GameObject>> GameObject::getChildren() const { return mChildren; }

/**
 * @brief Gets all components of the GameObject with a specific tag.
 *
 * @param aTag The tag to search for.
 *
 * @return A vector of references to the components with the specified tag.
 */
std::vector<std::reference_wrapper<Component>> GameObject::getComponentsWithTag(const std::string& aTag) const
{
	std::vector<std::reference_wrapper<Component>> componentsWithTag;
	for (const auto& component : mComponents)
	{
		if (component->getTag() == aTag)
		{
			componentsWithTag.push_back(*component);
		}
	}
	return componentsWithTag;
}