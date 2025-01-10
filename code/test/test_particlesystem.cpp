#include "EngineBravo.h"
#include "GameObject.h"
#include "ParticleEmitter.h"
#include "ParticleSystem.h"
#include "Scene.h"
#include <gtest/gtest.h>

class ParticleSystemTest : public ::testing::Test
{
protected:
	ParticleSystem* particleSystem;
	Scene& scene{EngineBravo::getInstance().getSceneManager().createScene("Particle System Test Scene", 1)};
	GameObject* gameObject;
	ParticleEmitter* emitter;

	void SetUp() override
	{
		particleSystem = new ParticleSystem();

		// Create a GameObject and add a ParticleEmitter component
		gameObject = new GameObject();
		emitter = new ParticleEmitter(EmitterMode::Continuous, 10.0f, 0.1f, 2000, 2000, {1, 1}, {1, 1}, {{1, 0, 0}},
									  0.0f, 0.0f, 0.0f);
		gameObject->addComponent(emitter);

		// Add GameObject to the Scene and ParticleSystem
		scene.addGameObject(gameObject);
		particleSystem->addObject(*gameObject);
	}

	void TearDown() override
	{
		delete particleSystem;
		EngineBravo::getInstance().getSceneManager().removeScene("Particle System Test Scene");
	}
};

TEST_F(ParticleSystemTest, AddObject)
{
	GameObject additionalObject;
	particleSystem->addObject(additionalObject);

	// Verify the object was added
	auto& objects = particleSystem->getObjects();
	EXPECT_EQ(objects.size(), 2);
	EXPECT_TRUE(std::any_of(objects.begin(), objects.end(),
							[&additionalObject](const auto& obj) { return &obj.get() == &additionalObject; }));
}

TEST_F(ParticleSystemTest, RemoveObject)
{
	particleSystem->removeObject(*gameObject);

	// Verify the object was removed
	auto& objects = particleSystem->getObjects();
	EXPECT_TRUE(objects.empty());
}

TEST_F(ParticleSystemTest, Update_ActiveGameObjectAndEmitter)
{
	gameObject->setActive(true);
	emitter->setActive(true);

	// Call update and ensure no exceptions
	EXPECT_NO_THROW(particleSystem->update());
}

TEST_F(ParticleSystemTest, Update_InactiveGameObject)
{
	gameObject->setActive(false);

	// Call update and ensure inactive GameObject is skipped
	EXPECT_NO_THROW(particleSystem->update());
}

TEST_F(ParticleSystemTest, Update_InactiveEmitter)
{
	gameObject->setActive(true);
	emitter->setActive(false);

	// Call update and ensure inactive emitter is skipped
	EXPECT_NO_THROW(particleSystem->update());
}

TEST_F(ParticleSystemTest, Update_MultipleEmitters)
{
	auto additionalEmitter = new ParticleEmitter(EmitterMode::Continuous, 10.0f, 0.1f, 2000, 2000, {1, 1}, {1, 1},
												 {{0, 1, 0}}, 0.0f, 0.0f, 0.0f);
	gameObject->addComponent(additionalEmitter);

	gameObject->setActive(true);
	emitter->setActive(true);
	additionalEmitter->setActive(true);

	// Call update and ensure no exceptions
	EXPECT_NO_THROW(particleSystem->update());
}

TEST_F(ParticleSystemTest, ClearObjects)
{
	particleSystem->clearObjects();

	// Verify all objects are cleared
	auto& objects = particleSystem->getObjects();
	EXPECT_TRUE(objects.empty());
}
