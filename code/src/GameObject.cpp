#include "GameObject.h"

#include <iostream>
#include <memory>

#include "Component.h"
#include "EngineBravo.h"

GameObject::GameObject() : mParent(nullptr), mTransform(Transform()), mID(-1), mName(""), mTag(""), mIsActive(true) {}

GameObject::~GameObject() {
    mComponents.clear(); // unique_ptr automatically handles deletion
}

// Copy constructor
GameObject::GameObject(const GameObject& other)
    : mParent(other.mParent), mTransform(other.mTransform), mID(other.mID), mName(other.mName), mTag(other.mTag),
      mIsActive(other.mIsActive) {

    std::cout << "GameObject copy constructor called" << std::endl;
    // Deep copy each component using its copy constructor
    for (const auto& component : other.mComponents) {
        if (component) {
            mComponents.push_back(std::move(component->clone())); // Using copy constructor
            mComponents.back()->setGameObjectParent(this);
        }
    }
}

// Copy assignment operator
GameObject& GameObject::operator=(const GameObject& other) {
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
    for (const auto& component : other.mComponents) {
        if (component) {
            mComponents.push_back(std::move(component->clone())); // Using copy constructor
        }
    }

    return *this;
}

// Move constructor
GameObject::GameObject(GameObject&& other) noexcept
    : mParent(other.mParent), mComponents(std::move(other.mComponents)), mTransform(std::move(other.mTransform)),
      mID(other.mID), mName(std::move(other.mName)), mTag(std::move(other.mTag)), mIsActive(other.mIsActive) {
    other.mParent = nullptr;
    other.mID = 0;
    other.mIsActive = false;
}

// Move assignment operator
GameObject& GameObject::operator=(GameObject&& other) noexcept {
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
void GameObject::addComponent(Component* aComponent) {
    if (aComponent) {
        aComponent->setGameObjectParent(this);
        mComponents.push_back(std::unique_ptr<Component>(aComponent));
        EngineBravo::getInstance().addToUpdateObjects(*this);
    }
}

void GameObject::removeComponent(Component* component) {
    auto it = std::remove_if(mComponents.begin(), mComponents.end(),
                             [component](const std::unique_ptr<Component>& comp) { return comp.get() == component; });
    if (it != mComponents.end()) {
        mComponents.erase(it, mComponents.end()); // unique_ptr automatically deletes the component
        EngineBravo::getInstance().addToUpdateObjects(*this);
    }
}

void GameObject::setID(int id) { mID = id; }

int GameObject::getID() { return mID; }

void GameObject::setName(const std::string& name) { mName = name; }

std::string GameObject::getName() { return mName; }

void GameObject::setTag(const std::string& tag) { mTag = tag; }

std::string GameObject::getTag() { return mTag; }

void GameObject::setActive(bool isActive) { mIsActive = isActive; }

bool GameObject::isActive() { return mIsActive; }

Transform GameObject::getTransform() {
    if (mParent) {
        return mParent->getTransform() + mTransform;
    }
    return mTransform;
}

void GameObject::setTransform(Transform aNewTransform) { mTransform = aNewTransform; }

void GameObject::setParent(GameObject* parent) { mParent = parent; }

GameObject* GameObject::getParent() { return mParent; }

std::vector<Component*> GameObject::getComponentsWithTag(const std::string& tag) const {
    std::vector<Component*> componentsWithTag;
    for (const auto& component : mComponents) {
        if (component->getTag() == tag) {
            componentsWithTag.push_back(component.get());
        }
    }
    return componentsWithTag;
}
