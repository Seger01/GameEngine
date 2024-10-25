#include "GameObject.h"

#include <algorithm>

GameObject::GameObject() : mID(0), mIsActive(true) {}

GameObject::~GameObject() {
    // Clean up components
    for (auto component : mComponents) {
        delete component;
    }
    mComponents.resize(0);
}

void GameObject::addComponent(Component* aComponent) {
    if (aComponent) {
        mComponents.push_back(aComponent);
        aComponent->setGameObjectParent(this);
    }
}

bool GameObject::isActiveInWorld() {
    // Here we can add logic to determine if the object is active in the world
    return isActiveSelf(); // For now, we assume that if it's active itself, it's active in the world
}

bool GameObject::isActiveSelf() { return mIsActive; }

void GameObject::setID(int id) { mID = id; }

int GameObject::getID() { return mID; }

void GameObject::setName(std::string name) { mName = name; }

std::string GameObject::getName() { return mName; }

void GameObject::setTag(std::string tag) { mTag = tag; }

std::string GameObject::getTag() { return mTag; }

void GameObject::setIsActive(bool isActive) { mIsActive = isActive; }

bool GameObject::getIsActive() { return mIsActive; }

Transform& GameObject::getTransform() { return mTransform; }
