/**
 * @file ParticleEmitter.cpp
 * @brief ParticleEmitter class implementation file.
 */

#include "ParticleEmitter.h"

#include <cmath>
#include <cstdlib>
#include <iostream>

#include "GameObject.h"
#include "Time.h"

/**
 * @brief ParticleEmitter class constructor
 * @param aEmitterMode The mode of the emitter
 * @param aSpeed The speed of the particles
 * @param aAcceleration The acceleration of the particles
 * @param aMinLifeTimeMs The minimum lifetime of the particles in milliseconds
 * @param aMaxLifeTimeMs The maximum lifetime of the particles in milliseconds
 * @param aSize The size of the particles
 * @param aEndSize The end size of the particles
 * @param aRotation The rotation of the particles
 * @param angularVelocity The angular velocity of the particles
 * @param angularAcceleration The angular acceleration of the particles
 * @param aColorGradient The color gradient of the particles
 * @param aTag The tag of the particle emitter
 */
ParticleEmitter::ParticleEmitter(EmitterMode aEmitterMode, float aSpeed, float aAcceleration, int aMinLifeTimeMs,
								 int aMaxLifeTimeMs, Vector2 aSize, Vector2 aEndSize, float aRotation,
								 float angularVelocity, float angularAcceleration, std::vector<Color> aColorGradient,
								 std::string aTag)
	: Component{aTag}, mEmitterMode(aEmitterMode), mVelocity(aSpeed), mAcceleration(aAcceleration),
	  mMinLifeTimeMs(aMinLifeTimeMs), mMaxLifeTimeMs(aMaxLifeTimeMs), mSize(aSize), mEndSize(aEndSize),
	  mRotation(aRotation), mAngularVelocity(angularVelocity), mAngularAcceleration(angularAcceleration),
	  mColorGradient(aColorGradient)
{
	mParticlesPerSecond = 0;
	mMinAngle = 0;
	mMaxAngle = 0;
}

/**
 * @brief ParticleEmitter class destructor
 */
ParticleEmitter::~ParticleEmitter() { mParticles.resize(0); }

/**
 * @brief Clone method for the ParticleEmitter class
 * @return A unique pointer to a new ParticleEmitter object
 */
std::unique_ptr<Component> ParticleEmitter::clone() const { return std::make_unique<ParticleEmitter>(*this); }

/**
 * @brief Generate a random velocity for the particles
 * @param aMinSpeed The minimum speed of the particles
 * @param aMaxSpeed The maximum speed of the particles
 * @param aMinAngle The minimum angle of the particles
 * @param aMaxAngle The maximum angle of the particles
 * @return The generated velocity
 */
Vector2 ParticleEmitter::generateRandomVelocity(float minSpeed, float maxSpeed, int minAngle, int maxAngle)
{
	// Generate a random angle between minAngle and maxAngle
	float angle = (minAngle + (static_cast<float>(rand()) / RAND_MAX * (maxAngle - minAngle))) * (M_PI / 180.0f);
	angle = angle + mRelativeTransform.rotation;

	// Generate a random speed between minSpeed and maxSpeed
	float speed = minSpeed + static_cast<float>(rand()) / RAND_MAX * (maxSpeed - minSpeed);

	// Calculate velocity components
	Vector2 velocity;
	velocity.x = std::cos(angle) * speed;
	velocity.y = std::sin(angle) * speed;

	return velocity;
}

/**
 * @brief Set the relative transform of the particle emitter
 * @param aTransform The new relative transform of the particle emitter
 */
void ParticleEmitter::setRelativeTransform(Transform aTransform) { mRelativeTransform = aTransform; }

/**
 * @brief Get the relative transform of the particle emitter
 * @return The relative transform of the particle emitter
 */
Transform& ParticleEmitter::getRelativeTransform() { return mRelativeTransform; }

/**
 * @brief Set the amount of particles to spawn per second
 * @param aParticlesPerSecond The amount of particles to spawn per second
 */
void ParticleEmitter::setParticlesPerSecond(int aParticlesPerSecond) { mParticlesPerSecond = aParticlesPerSecond; }

/**
 * @brief Set the angle of the particles
 * @param aMinAngle The minimum angle of the particles
 * @param aMaxAngle The maximum angle of the particles
 */
void ParticleEmitter::setAngle(int aMinAngle, int aMaxAngle)
{
	mMinAngle = aMinAngle;
	mMaxAngle = aMaxAngle;

	if (mMinAngle < 0)
	{
		mMinAngle = 0;
	}

	if (mMaxAngle > 360)
	{
		mMaxAngle = 360;
	}

	if (mMinAngle > mMaxAngle)
	{
		std::swap(mMinAngle, mMaxAngle);
	}
}

/**
 * @brief Get the minimum angle of the particles
 * @return The minimum angle of the particles
 */
int ParticleEmitter::getMinAngle() { return mMinAngle; }

/**
 * @brief Get the maximum angle of the particles
 * @return The maximum angle of the particles
 */
int ParticleEmitter::getMaxAngle() { return mMaxAngle; }

/**
 * @brief Spawn a particle
 */
void ParticleEmitter::spawnParticle()
{
	Vector2 particlePos;
	if (mGameObject != nullptr)
	{
		particlePos = mGameObject->getTransform().position + mRelativeTransform.position;
	}
	else
	{
		particlePos = mRelativeTransform.position;
	}

	Vector2 particleVel = generateRandomVelocity(mVelocity, mVelocity, mMinAngle, mMaxAngle);
	float particleAccel = mAcceleration;

	int particleLifeTime = mMinLifeTimeMs + (rand() % (mMaxLifeTimeMs - mMinLifeTimeMs + 1));

	Vector2 particleSize = mSize;
	Vector2 particleEndSize = mEndSize;
	float particleRotation = mRotation;
	float particleAngularVelocity = mAngularVelocity;
	float particleAngularAcceleration = mAngularAcceleration;
	std::vector<Color> particleColorGradient = mColorGradient;

	Particle particle(particlePos, particleVel, particleAccel, particleLifeTime, mMaxLifeTimeMs, particleSize,
					  particleEndSize, particleRotation, particleAngularVelocity, particleAngularAcceleration,
					  particleColorGradient);

	mParticles.push_back(particle);
}

/**
 * @brief Update method for the ParticleEmitter class
 */
void ParticleEmitter::update()
{
	if (mActive)
	{
		if (mEmitterMode == EmitterMode::Continuous)
		{
			static float amountOfParticlesToSpawn;
			amountOfParticlesToSpawn += mParticlesPerSecond * Time::deltaTime;
			int i = 0;
			for (i = 0; i < amountOfParticlesToSpawn; i++)
			{
				spawnParticle();
			}
			amountOfParticlesToSpawn -= i;
		}
	}

	for (int i = 0; i < mParticles.size();)
	{ // Note: no increment here
		mParticles[i].update();
		if (mParticles[i].getLifeTime() <= 0)
		{
			mParticles.erase(mParticles.begin() + i); // Erase the element at index i
		}
		else
		{
			++i; // Only increment if no element was erased
		}
	}
	// for (int i = 0; i < mParticles.size();) {
	//     mParticles[i].update();
	//     if (mParticles[i].getLifeTime() <= 0) {
	//         std::swap(mParticles[i], mParticles.back());
	//         mParticles.pop_back(); // Removes the last element efficiently
	//     } else {
	//         ++i;
	//     }
	// }
}

/**
 * @brief Burst method for the ParticleEmitter class
 * @param aAmount The amount of particles to spawn
 */
void ParticleEmitter::burst(int aAmount)
{
	for (int i = 0; i < aAmount; i++)
	{
		spawnParticle();
	}
}

/**
 * @brief Get the particles of the particle emitter
 * @return The particles of the particle emitter
 */
std::vector<Particle>& ParticleEmitter::getParticles() { return mParticles; }

/**
 * @brief Set the layer of the particle emitter
 * @param aLayer The new layer of the particle emitter
 */
void ParticleEmitter::setLayer(int aLayer) { mLayer = aLayer; }

/**
 * @brief Get the layer of the particle emitter
 * @return The layer of the particle emitter
 */
int ParticleEmitter::getLayer() { return mLayer; }
