/**
 * @file ParticleEmitter.h
 * @brief ParticleEmitter class header file.
 */

#ifndef PARTICLEEMITTER_H
#define PARTICLEEMITTER_H

#include <vector>

#include "Component.h"
#include "Particle.h"

/**
 * @class ParticleEmitter
 * @brief Class for emitting particles.
 */
enum EmitterMode
{
	Continuous,
	Burst,
};

/**
 * @class ParticleEmitter
 * @brief Class for emitting particles.
 */

class ParticleEmitter : public Component
{
public:
	ParticleEmitter(EmitterMode aEmitterMode, float aSpeed, float aAcceleration, int aMinLifeTimeMs, int aMaxLifeTimeMs,
					Vector2 aSize, Vector2 aEndSize, std::vector<Color> aColorGradient, float aRotation = 0.0f,
					float angularVelocity = 0.0f, float angularAcceleration = 0.0f,
					std::string aTag = "defaultParticleEmitter");

	~ParticleEmitter();

	std::unique_ptr<Component> clone() const override;

	void update();

	void burst(int aAmount);

	void setRelativeTransform(Transform aTransform);
	Transform& getRelativeTransform();

	void setParticlesPerSecond(int aParticlesPerSecond);
	void setAngle(int aMinAngle, int aMaxAngle);

	int getMinAngle();
	int getMaxAngle();

	std::vector<Particle>& getParticles();

	void setLayer(int aLayer);
	int getLayer();

private:
	void spawnParticle();

	Vector2 generateRandomVelocity(float aMinSpeed, float aMaxSpeed, int aMinAngle, int aMaxAngle);

private:
	Transform mRelativeTransform; // Relative transform of the particle emitter

	int mParticlesPerSecond;		  // Amount of particles to spawn per second
	std::vector<Particle> mParticles; // Vector of particles

	EmitterMode mEmitterMode; // Mode of the emitter

	int mMinAngle; // Min angle of the particles
	int mMaxAngle; // Max angle of the particles

	int mMinLifeTimeMs; // Min lifetime of the particles
	int mMaxLifeTimeMs; // Max lifetime of the particles

	float mVelocity;				   // Velocity of the particles
	float mAcceleration;			   // Acceleration of the particles
	Vector2 mSize;					   // Size of the particles
	Vector2 mEndSize;				   // End size of the particles
	float mRotation;				   // Rotation of the particles
	float mAngularVelocity;			   // Angular velocity of the particles
	float mAngularAcceleration;		   // Angular acceleration of the particles
	std::vector<Color> mColorGradient; // Color gradient of the particles

	int mLayer = 0; // Layer of the particles
};

#endif // PARTICLEEMITTER_H
