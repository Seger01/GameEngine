/**
 * @file Component.cpp
 *
 * @brief This file contains the implementation of the Component class
 */
#include "Components/Component.h"

/**
 * @brief Component constructor. Sets the tag to the argument value, and sets the component to active.
 *
 * @param aTag The tag of the component. Defaults to "defaultComponent".
 */
Component::Component(const std::string& aTag) : mGameObject{nullptr}, mTag(aTag), mActive(true) {}

/**
 * @brief Clone the component. Mandatory for the prototype pattern, and all derived classes should override this.
 */
std::unique_ptr<Component> Component::clone() const { return std::make_unique<Component>(*this); }

/**
 * @brief Set the parent GameObject of the component.
 *
 * @param aParentObject The GameObject to set as the parent.
 */
void Component::setGameObjectParent(GameObject* aParentObject) { mGameObject = aParentObject; }

/**
 * @brief Check if the component is active
 *
 * @return true if the component is active, false otherwise
 */
bool Component::isActive() { return mActive; }

/**
 * @brief Set the active state of the component
 *
 * @param aState The state to set the component to
 */
void Component::setActive(bool aState) { mActive = aState; }

/**
 * @brief Get the tag of the component
 */
std::string Component::getTag() { return mTag; }

/**
 * @brief Set the tag of the component
 *
 * @param aTag The tag to set
 */
void Component::setTag(const std::string& aTag) { mTag = aTag; }
