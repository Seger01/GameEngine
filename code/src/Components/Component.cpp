#include "Component.h"

Component::Component(std::string aTag) : mTag(aTag) {}

// Copy constructor
Component::Component(const Component& other)
    : mGameObject(other.mGameObject), mTag(other.mTag), mActive(other.mActive) {}

// Copy assignment operator
Component& Component::operator=(const Component& other) {
    if (this == &other)
        return *this;

    mGameObject = other.mGameObject;
    mTag = other.mTag;
    mActive = other.mActive;

    return *this;
}

// Move constructor
Component::Component(Component&& other) noexcept
    : mGameObject(other.mGameObject), mTag(std::move(other.mTag)), mActive(other.mActive) {
    other.mGameObject = nullptr;
}

// Move assignment operator
Component& Component::operator=(Component&& other) noexcept {
    if (this == &other)
        return *this;

    mGameObject = other.mGameObject;
    mTag = std::move(other.mTag);
    mActive = other.mActive;

    other.mGameObject = nullptr;

    return *this;
}
