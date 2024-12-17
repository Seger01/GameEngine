#include "GameObject.h"

#include <functional>
#include <iostream>
#include <memory>

#include "Component.h"
#include "EngineBravo.h"

GameObject::GameObject() : mParent(nullptr), mTransform(Transform()), mID(-1), mName(""), mTag(""), mIsActive(true) {}

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

// Copy constructor
GameObject::GameObject(const GameObject& other)
	: mParent(other.mParent), mTransform(other.mTransform), mID(other.mID), mName(other.mName), mTag(other.mTag),
	  mIsActive(other.mIsActive)
{
	// Deep copy each component using its copy constructor
	for (const auto& component : other.mComponents)
	{
		if (component)
		{
			mComponents.push_back(std::move(component->clone())); // Using copy constructor
			mComponents.back()->setGameObjectParent(this);
		}
	}
}

// Copy assignment operator
GameObject& GameObject::operator=(const GameObject& other)
{
	if (this == &other)
		return *this;

	mParent = other.mParent;
	mTransform = other.mTransform;
	mID = other.mID;
	mName = other.mName;
	mTag = other.mTag;
	mIsActive = other.mIsActive;

	// Clear and deep copy each component using its copy constructor
	mComponents.clear();
	for (const auto& component : other.mComponents)
	{
		if (component)
		{
			mComponents.push_back(std::move(component->clone())); // Using copy constructor
		}
	}

	return *this;
}

// Move constructor
GameObject::GameObject(GameObject&& other) noexcept
	: mParent(other.mParent), mComponents(std::move(other.mComponents)), mTransform(std::move(other.mTransform)),
	  mID(other.mID), mName(std::move(other.mName)), mTag(std::move(other.mTag)), mIsActive(other.mIsActive)
{
	other.mParent = nullptr;
	other.mID = 0;
	other.mIsActive = false;
}

// Move assignment operator
GameObject& GameObject::operator=(GameObject&& other) noexcept
{
	if (this == &other)
		return *this;

	mParent = other.mParent;
	mComponents = std::move(other.mComponents);
	mTransform = std::move(other.mTransform);
	mID = other.mID;
	mName = std::move(other.mName);
	mTag = std::move(other.mTag);
	mIsActive = other.mIsActive;

	other.mParent = nullptr;
	other.mID = 0;
	other.mIsActive = false;

	return *this;
}

void GameObject::addComponent(Component* aComponent)
{
	aComponent->setGameObjectParent(this);
	mComponents.push_back(std::unique_ptr<Component>(aComponent));
	EngineBravo::getInstance().getUpdateQueue().addToUpdateObjects(*this);
}

void GameObject::removeComponent(Component* component)
{
	auto it = std::remove_if(mComponents.begin(), mComponents.end(),
							 [component](const std::unique_ptr<Component>& comp) { return comp.get() == component; });
	if (it != mComponents.end())
	{
		mComponents.erase(it, mComponents.end()); // unique_ptr automatically deletes the component
		EngineBravo::getInstance().getUpdateQueue().addToUpdateObjects(*this);
	}
}

void GameObject::setID(int id) { mID = id; }

int GameObject::getID() const { return mID; }

void GameObject::setName(const std::string& name) { mName = name; }

std::string GameObject::getName() const { return mName; }

void GameObject::setTag(const std::string& tag) { mTag = tag; }

std::string GameObject::getTag() const { return mTag; }

/**
 * @brief Sets the active state of the GameObject and all its children.
 */
void GameObject::setActive(bool isActive)
{
	mIsActive = isActive;
	for (GameObject& child : mChildren)
	{
		child.setActive(isActive);
	}
}

bool GameObject::isActive() const { return mIsActive; }

Transform GameObject::getTransform() const
{
	if (mParent)
	{
		Transform parentTransform = mParent->getTransform();

		parentTransform.position += mTransform.position;
		parentTransform.rotation += mTransform.rotation;
		parentTransform.scale = parentTransform.scale * mTransform.scale;

		return parentTransform;
	}
	return mTransform;
}

Transform& GameObject::getTransformRef() { return mTransform; }

void GameObject::setTransform(Transform aNewTransform) { mTransform = aNewTransform; }

void GameObject::setParent(GameObject& parent)
{
	if (mParent)
	{
		mParent->removeChild(*this);
	}
	mParent = &parent;
	mParent->addChild(*this);
}

void GameObject::removeParent() { mParent = nullptr; }

GameObject& GameObject::getParent()
{
	if (mParent == nullptr)
	{
		throw std::runtime_error("Parent is null");
	}
	return *mParent;
}

void GameObject::addChild(GameObject& child) { mChildren.push_back(child); }

void GameObject::removeChild(GameObject& child)
{
	auto it = std::remove_if(mChildren.begin(), mChildren.end(),
							 [&child](std::reference_wrapper<GameObject>& ref) { return &ref.get() == &child; });
	if (it != mChildren.end())
	{
		mChildren.erase(it, mChildren.end());
	}
}

std::vector<std::reference_wrapper<GameObject>> GameObject::getChildren() const { return mChildren; }

std::vector<std::reference_wrapper<Component>> GameObject::getComponentsWithTag(const std::string& tag) const
{
	std::vector<std::reference_wrapper<Component>> componentsWithTag;
	for (const auto& component : mComponents)
	{
		if (component->getTag() == tag)
		{
			componentsWithTag.push_back(*component);
		}
	}
	return componentsWithTag;
}
