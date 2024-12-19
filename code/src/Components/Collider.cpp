#include "Collider.h"

/**
 * @file Collider.cpp
 *
 * @brief This file contains the implementation of the Collider class which is used to
 */

Collider::Collider(const std::string& aTag) : Component(aTag) {}

std::unique_ptr<Component> Collider::clone() const { return std::make_unique<Collider>(*this); }

Transform Collider::getTransform() const { return mTransform; }

void Collider::setTransform(const Transform& transform) { mTransform = transform; }
