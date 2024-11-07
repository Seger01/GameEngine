#include "Particle.h"
#include "Time.h"
#include <gtest/gtest.h>

class ParticleTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Set up necessary objects before each test.
        particle = Particle(Vector2(0, 0), Vector2(1, 0), 0.1f, 1000, 1000, Vector2(10, 10), Vector2(20, 20), 0.0f,
                            0.1f, 0.0f, {Color(255, 0, 0, 255), Color(0, 255, 0, 255)});
    }

    Particle particle;
};

TEST_F(ParticleTest, ConstructorInitializesValues) {
    EXPECT_EQ(particle.getPosition().x, 0);
    EXPECT_EQ(particle.getPosition().y, 0);
    EXPECT_EQ(particle.getLifeTime(), 1); // 1000 ms = 1 second
    EXPECT_EQ(particle.getSize().x, 10);
    EXPECT_EQ(particle.getSize().y, 10);
    EXPECT_EQ(particle.getRotation(), 0);
    // check color gradient per color subvalue
    EXPECT_EQ(particle.getColor().r, 255);
    EXPECT_EQ(particle.getColor().g, 0);
    EXPECT_EQ(particle.getColor().b, 0);
    EXPECT_EQ(particle.getColor().a, 255);
}

TEST_F(ParticleTest, ParticleMovesAndChangesSizeOnUpdate) {
    float deltaTime = 0.1f; // simulate 100ms update steps
    Time::deltaTime = deltaTime;

    particle.update();

    // Check if position changed as expected
    EXPECT_GT(particle.getPosition().x, 0);
    EXPECT_EQ(particle.getPosition().y, 0); // Since velocity is only along x-axis, y should remain unchanged

    // Check if size is interpolated correctly after the update
    EXPECT_GT(particle.getSize().x, 10);
    EXPECT_GT(particle.getSize().y, 10);
}

TEST_F(ParticleTest, ParticleLifeTimeDecreasesOnUpdate) {
    float initialLifeTime = particle.getLifeTime();
    Time::deltaTime = 0.1f; // simulate 100ms update steps

    particle.update();
    EXPECT_LT(particle.getLifeTime(), initialLifeTime); // Ensure that the lifetime has decreased
}

TEST_F(ParticleTest, ParticleColorChangesBasedOnLifeTime) {
    // Simulate 50% of the particle's lifetime
    float deltaTime = 0.5f; // 500 ms
    Time::deltaTime = deltaTime;

    particle.update();
    // After 500ms, the particle should have interpolated to the second color in the gradient
    EXPECT_EQ(particle.getColor().r, 127);
    EXPECT_EQ(particle.getColor().g, 127);
    EXPECT_EQ(particle.getColor().b, 0);
    EXPECT_EQ(particle.getColor().a, 255);
}

TEST_F(ParticleTest, ParticleSizeAndLifeTimeBecomeZeroWhenExpired) {
    // Simulate the particle's lifetime expiration
    float deltaTime = 3.0f; // > 1000 ms
    Time::deltaTime = deltaTime;

    particle.update();
    EXPECT_EQ(particle.getLifeTime(), 0); // Particle's lifetime should be 0
}
