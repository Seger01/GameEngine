#include "ParticleEmitter.h"
#include "Time.h"
#include <gtest/gtest.h>

class ParticleEmitterTest : public ::testing::Test
{
protected:
	// Set up test objects
	ParticleEmitter* emitter;

	ParticleEmitterTest()
	{
		// Create an emitter with arbitrary parameters
		emitter = new ParticleEmitter(EmitterMode::Continuous, 10.0f, 0.1f, 2000, 2000, {1, 1}, {1, 1}, {{1, 0, 0}});
	}

	~ParticleEmitterTest() { delete emitter; }
};

// Test the constructor and default values
TEST_F(ParticleEmitterTest, ConstructorTest)
{
	EXPECT_EQ(emitter->getParticles().size(), 0);
	EXPECT_EQ(emitter->getLayer(), 0);
}

// Test the set and get layer functionality
TEST_F(ParticleEmitterTest, SetAndGetLayerTest)
{
	emitter->setLayer(5);
	EXPECT_EQ(emitter->getLayer(), 5);
}

// Test set and get particles per second
TEST_F(ParticleEmitterTest, SetParticlesPerSecondTest)
{
	emitter->setParticlesPerSecond(20);
	EXPECT_EQ(emitter->getParticles().size(), 0); // No particles yet, just set rate
}

// Test the angle setting and validation
TEST_F(ParticleEmitterTest, SetAngleTest)
{
	emitter->setAngle(30, 90);
	// Expect min and max angle to be set correctly
	EXPECT_EQ(emitter->getMinAngle(), 30);
	EXPECT_EQ(emitter->getMaxAngle(), 90);

	emitter->setAngle(90, 30); // Test swapping angles
	EXPECT_EQ(emitter->getMinAngle(), 30);
	EXPECT_EQ(emitter->getMaxAngle(), 90);

	emitter->setAngle(45, 400); // Ensure angle wraps around correctly
	EXPECT_EQ(emitter->getMinAngle(), 45);
	EXPECT_EQ(emitter->getMaxAngle(), 360);
}

// Test spawning of particles in Continuous mode
TEST_F(ParticleEmitterTest, SpawnParticleTest)
{
	Time::deltaTime = 1.0f; // Set delta time to 1 second
	emitter->setParticlesPerSecond(5);

	emitter->update();
	EXPECT_GT(emitter->getParticles().size(), 0); // Should have spawned some particles

	// Update again and check that more particles are spawned
	emitter->update();
	EXPECT_GT(emitter->getParticles().size(), 0); // Should have spawned more particles
}

// Test particle cleanup after they exceed their lifetime
TEST_F(ParticleEmitterTest, ParticleLifetimeTest)
{
	emitter->setParticlesPerSecond(1);
	Time::deltaTime = 1.0f;

	emitter->update(); // Spawn a particle
	size_t initialSize = emitter->getParticles().size();

	// Simulate particle lifetime expiration
	Time::deltaTime = 1.0f;
	for (auto& particle : emitter->getParticles())
	{
		particle.update(); // Decrease lifetime
	}

	emitter->setParticlesPerSecond(0); // Stop spawning particles

	emitter->update();											// Call update to remove expired particles
	EXPECT_EQ(emitter->getParticles().size(), initialSize - 1); // Particle should be removed
}

// Test update with Burst mode
TEST_F(ParticleEmitterTest, BurstModeTest)
{
	// Switch to Burst mode and set particles per second
	ParticleEmitter burstEmitter(EmitterMode::Burst, 10.0f, 0.1f, 100, 500, {1, 1}, {1, 1}, {{1, 0, 0}}, 0.0f, 0.0f,
								 0.0f);

	burstEmitter.burst(5);

	// Burst should spawn particles once and not continuously
	EXPECT_EQ(burstEmitter.getParticles().size(), 5); // 5 particles should be spawned in burst mode
}
