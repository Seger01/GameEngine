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
 * @enum EmitterMode
 * @brief Enum for the mode of the emitter.
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
	/// @brief Relative transform of the particle emitter.
	Transform mRelativeTransform;

	/// @brief The number of particles to emit per second.
	int mParticlesPerSecond;
	/// @brief The particles that were spawned by this emitter
	std::vector<Particle> mParticles;

	/// @brief The mode of the emitter.
	EmitterMode mEmitterMode;

	/// @brief The minimum and maximum angle of the particles.
	int mMinAngle;
	int mMaxAngle;

	/// @brief the minimum lifetime of the particles.
	int mMinLifeTimeMs;
	/// @brief the maximum lifetime of the particles.
	int mMaxLifeTimeMs;

	/// @brief The speed of the particles.
	float mVelocity;
	/// @brief The acceleration of the particles.
	float mAcceleration;
	/// @brief The size of the particles.
	Vector2 mSize;
	/// @brief The size the particles are at the end of the maximum lifetime.
	Vector2 mEndSize;
	/// @brief The starting rotation of the particles.
	float mRotation;
	/// @brief The angular velocity of the particles.
	float mAngularVelocity;
	/// @brief The angular acceleration of the particles.
	float mAngularAcceleration;
	/// @brief The color gradient of the particles.
	std::vector<Color> mColorGradient;

	/// @brief The layer of the particles.
	int mLayer = 0;
};

#endif // PARTICLEEMITTER_H
