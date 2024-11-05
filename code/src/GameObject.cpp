#include "GameObject.h"

GameObject::GameObject() : mID(0), mIsActive(true) {}

GameObject::~GameObject() {
    for (auto component : mComponents) {
        delete component;
    }
    mComponents.clear();
}

void GameObject::addComponent(Component* aComponent) {
    if (aComponent) {
        mComponents.push_back(aComponent);
        aComponent->setGameObjectParent(this);
    }
}

void GameObject::removeComponent(Component* component) {
    if (component) {
        auto it = std::find(mComponents.begin(), mComponents.end(), component);
        if (it != mComponents.end()) {
            delete *it; // Free memory if the component was dynamically allocated
            mComponents.erase(it);
        }
    }
}

bool GameObject::isActiveInWorld() { return isActiveSelf(); }

bool GameObject::isActiveSelf() { return mIsActive; }

void GameObject::setID(int id) { mID = id; }

int GameObject::getID() { return mID; }

void GameObject::setName(const std::string& name) { mName = name; }

std::string GameObject::getName() { return mName; }

void GameObject::setTag(const std::string& tag) { mTag = tag; }

std::string GameObject::getTag() { return mTag; }

void GameObject::setActive(bool isActive) { mIsActive = isActive; }

bool GameObject::isActive() { return mIsActive; }

Transform GameObject::getTransform() { return mTransform; }

void GameObject::setTransform(Transform aNewTransform) { mTransform = aNewTransform; }

std::vector<Component*> GameObject::getComponentsWithTag(const std::string& tag) const {
    std::vector<Component*> componentsWithTag;
    for (const auto& component : mComponents) {
        if (component->getTag() == tag) {
            componentsWithTag.push_back(component);
        }
    }
    return componentsWithTag;
}
