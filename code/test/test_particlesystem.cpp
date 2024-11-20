#include "EngineBravo.h"
#include "GameObject.h"
#include "ParticleEmitter.h"
#include "ParticleSystem.h"
#include "Scene.h"
#include <gtest/gtest.h>

class ParticleSystemTest : public ::testing::Test {
protected:
    ParticleSystem* particleSystem;
    Scene* scene;
    GameObject* gameObject;
    ParticleEmitter* emitter;

    void SetUp() override {
        particleSystem = new ParticleSystem();
        scene = EngineBravo::getInstance().getSceneManager().createScene("Particle System Test Scene", 1);

        // Create a GameObject and add a ParticleEmitter component
        gameObject = new GameObject();
        emitter = new ParticleEmitter(EmitterMode::Continuous, 10.0f, 0.1f, 2000, 2000, {1, 1}, {1, 1}, 0.0f, 0.0f,
                                      0.0f, {{1, 0, 0}});
        gameObject->addComponent(emitter);

        // Add GameObject to the Scene
        scene->addGameObject(gameObject);
    }

    void TearDown() override { EngineBravo::getInstance().getSceneManager().removeScene("Particle System Test Scene"); }
};

TEST_F(ParticleSystemTest, Update_ActiveGameObjectAndEmitter) {
    // Ensure the GameObject and emitter are active
    gameObject->setActive(true);
    emitter->setActive(true);

    // Call update and verify emitter's update method is invoked
    EXPECT_NO_THROW(particleSystem->update(scene));
}

TEST_F(ParticleSystemTest, Update_InactiveGameObject) {
    // Set GameObject as inactive
    gameObject->setActive(false);

    // Update ParticleSystem - emitter update should not be called
    EXPECT_NO_THROW(particleSystem->update(scene));
}

TEST_F(ParticleSystemTest, Update_InactiveEmitter) {
    // Set GameObject active but emitter inactive
    gameObject->setActive(true);
    emitter->setActive(false);

    // Update ParticleSystem - inactive emitter should not be updated
    EXPECT_NO_THROW(particleSystem->update(scene));
}

TEST_F(ParticleSystemTest, Update_MultipleEmitters) {
    // Add additional ParticleEmitter to the same GameObject
    ParticleEmitter* additionalEmitter = new ParticleEmitter(EmitterMode::Continuous, 10.0f, 0.1f, 2000, 2000, {1, 1},
                                                             {1, 1}, 0.0f, 0.0f, 0.0f, {{1, 0, 0}});
    gameObject->addComponent(additionalEmitter);

    // Ensure both emitters are active
    gameObject->setActive(true);
    emitter->setActive(true);
    additionalEmitter->setActive(true);

    // Update ParticleSystem
    EXPECT_NO_THROW(particleSystem->update(scene));
}

TEST_F(ParticleSystemTest, Update_NoEmitters) {
    // Remove all emitters from GameObject
    ParticleEmitter* emitterComponent = gameObject->getComponents<ParticleEmitter>()[0];
    gameObject->removeComponent(emitterComponent);

    // Update ParticleSystem - should handle gracefully with no emitters
    EXPECT_NO_THROW(particleSystem->update(scene));
}
