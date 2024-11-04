#include "ParticleSystem.h"

#include "ParticleEmitter.h"

ParticleSystem::ParticleSystem() {}

void ParticleSystem::update(Scene* aScene) {
    for (auto& gameObject : aScene->getGameObjects()) {
        if (gameObject->isActive()) {
            if (gameObject->hasComponent<ParticleEmitter>()) {
                for (auto particleEmitter : gameObject->getComponents<ParticleEmitter>()) {
                    particleEmitter->update();
                }
            }
        }
    }
}
