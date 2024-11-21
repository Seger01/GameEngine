#include "ParticleSystem.h"

#include "ParticleEmitter.h"

ParticleSystem::ParticleSystem() {}

void ParticleSystem::update(Scene* aScene) {
    for (GameObject& gameObject : mObjects) {
        if (gameObject.isActive()) {
            // if (gameObject->hasComponent<ParticleEmitter>()) {
            for (auto particleEmitter : gameObject.getComponents<ParticleEmitter>()) {
                particleEmitter->update();
            }
            //}
        }
    }
}

void ParticleSystem::addObject(GameObject& aObject) {
    auto it =
        std::find_if(mObjects.begin(), mObjects.end(), [&aObject](const std::reference_wrapper<GameObject>& wrapper) {
            return &wrapper.get() == &aObject; // Compare addresses
        });
    if (it == mObjects.end()) {
        // Object has not been added yet
        mObjects.push_back(aObject);
    }
}

void ParticleSystem::removeObject(GameObject& aObject) {
    auto it =
        std::remove_if(mObjects.begin(), mObjects.end(),
                       [&aObject](const std::reference_wrapper<GameObject>& obj) { return &obj.get() == &aObject; });
    if (it != mObjects.end()) {
        mObjects.erase(it, mObjects.end());
    }
}
