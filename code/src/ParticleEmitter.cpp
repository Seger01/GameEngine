#include "ParticleEmitter.h"

#include <cmath>
#include <cstdlib>
#include <iostream>

#include "GameObject.h"
#include "Time.h"

ParticleEmitter::ParticleEmitter(EmitterMode aEmitterMode, float aSpeed, float aAcceleration, int aMinLifeTimeMs,
                                 int aMaxLifeTimeMs, Vector2 aSize, Vector2 aSizeShift, float aRotation,
                                 float angularVelocity, float angularAcceleration, std::vector<Color> aColorGradient)
    : mEmitterMode(aEmitterMode), mVelocity(aSpeed), mAcceleration(aAcceleration), mMinLifeTimeMs(aMinLifeTimeMs),
      mMaxLifeTimeMs(aMaxLifeTimeMs), mSize(aSize), mSizeShift(aSizeShift), mRotation(aRotation),
      mAngularVelocity(angularVelocity), mAngularAcceleration(angularAcceleration), mColorGradient(aColorGradient) {
    mParticlesPerSecond = 0;
    mMinAngle = 0;
    mMaxAngle = 0;
}

ParticleEmitter::~ParticleEmitter() { mParticles.resize(0); }

Vector2 ParticleEmitter::generateRandomVelocity(float minSpeed, float maxSpeed, int minAngle, int maxAngle) {
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

void ParticleEmitter::setRelativeTransform(Transform aTransform) { mRelativeTransform = aTransform; }

Transform& ParticleEmitter::getRelativeTransform() { return mRelativeTransform; }

void ParticleEmitter::setParticlesPerSecond(int aParticlesPerSecond) { mParticlesPerSecond = aParticlesPerSecond; }

void ParticleEmitter::setAngle(int aMinAngle, int aMaxAngle) {
    mMinAngle = aMinAngle;
    mMaxAngle = aMaxAngle;

    if (mMinAngle > mMaxAngle) {
        std::swap(mMinAngle, mMaxAngle);
    }

    if (mMaxAngle - mMinAngle > 360) {
        mMaxAngle = 0;
        mMaxAngle = mMinAngle + 360;
    }
}

void ParticleEmitter::spawnParticle() {
    // Particle particle(Vector2(0, 0), Vector2(0, 0), acceleration, lifeTime, size, rotation, angularVelocity,
    //                   angularAcceleration, colorGradient[0]);

    Vector2 particlePos = mGameObject->getTransform().position + mRelativeTransform.position;
    Vector2 particleVel = generateRandomVelocity(mVelocity, mVelocity, mMinAngle, mMaxAngle);
    float particleAccel = mAcceleration;

    int particleLifeTime = mMinLifeTimeMs + (rand() % (mMaxLifeTimeMs - mMinLifeTimeMs + 1));

    Vector2 particleSize = mSize;
    Vector2 particleSizeShift = mSizeShift;
    float particleRotation = mRotation;
    float particleAngularVelocity = mAngularVelocity;
    float particleAngularAcceleration = mAngularAcceleration;
    std::vector<Color> particleColorGradient = mColorGradient;

    Particle particle(particlePos, particleVel, particleAccel, particleLifeTime, mMaxLifeTimeMs, particleSize,
                      particleSizeShift, particleRotation, particleAngularVelocity, particleAngularAcceleration,
                      particleColorGradient);

    mParticles.push_back(particle);
}

void ParticleEmitter::update() {
    if (mEmitterMode == EmitterMode::Continuous) {
        int amountOfParticlesToSpawn = (mParticlesPerSecond * Time::deltaTime) / 1000;
        for (int i = 0; i < amountOfParticlesToSpawn; i++) {
            spawnParticle();
        }
    }

    for (int i = 0; i < mParticles.size();) { // Note: no increment here
        mParticles[i].update();
        if (mParticles[i].getLifeTime() <= 0) {
            mParticles.erase(mParticles.begin() + i); // Erase the element at index i
        } else {
            ++i; // Only increment if no element was erased
        }
    }
}

std::vector<Particle>& ParticleEmitter::getParticles() { return mParticles; }
