#include "GameObject.h"

#include <memory>

#include "Component.h"

GameObject::GameObject() : mParent(nullptr), mTransform(Transform()), mID(-1), mName(""), mTag(""), mIsActive(true) {}

GameObject::~GameObject() {
    mComponents.clear(); // unique_ptr automatically handles deletion
}

void GameObject::addComponent(Component* aComponent) {
    if (aComponent) {
        aComponent->setGameObjectParent(this);
        mComponents.push_back(std::unique_ptr<Component>(aComponent));
    }
}

void GameObject::removeComponent(Component* component) {
    auto it = std::remove_if(mComponents.begin(), mComponents.end(),
                             [component](const std::unique_ptr<Component>& comp) { return comp.get() == component; });
    if (it != mComponents.end()) {
        mComponents.erase(it, mComponents.end()); // unique_ptr automatically deletes the component
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
