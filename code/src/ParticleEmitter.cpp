#include "ParticleEmitter.h"

#include <cmath>
#include <cstdlib>
#include <iostream>

#include "GameObject.h"
#include "Time.h"

ParticleEmitter::ParticleEmitter(EmitterMode aEmitterMode, float aSpeed, float aAcceleration, int aMinLifeTimeMs,
                                 int aMaxLifeTimeMs, Vector2 aSize, Vector2 aEndSize, float aRotation,
                                 float angularVelocity, float angularAcceleration, std::vector<Color> aColorGradient,
                                 std::string aTag)
    : Component{aTag}, mEmitterMode(aEmitterMode), mVelocity(aSpeed), mAcceleration(aAcceleration),
      mMinLifeTimeMs(aMinLifeTimeMs), mMaxLifeTimeMs(aMaxLifeTimeMs), mSize(aSize), mEndSize(aEndSize),
      mRotation(aRotation), mAngularVelocity(angularVelocity), mAngularAcceleration(angularAcceleration),
      mColorGradient(aColorGradient) {
    mParticlesPerSecond = 0;
    mMinAngle = 0;
    mMaxAngle = 0;
}

ParticleEmitter::~ParticleEmitter() { mParticles.resize(0); }

std::unique_ptr<Component> ParticleEmitter::clone() const { return std::make_unique<ParticleEmitter>(*this); }

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

    if (mMinAngle < 0) {
        mMinAngle = 0;
    }

    if (mMaxAngle > 360) {
        mMaxAngle = 360;
    }

    if (mMinAngle > mMaxAngle) {
        std::swap(mMinAngle, mMaxAngle);
    }
}

int ParticleEmitter::getMinAngle() { return mMinAngle; }

int ParticleEmitter::getMaxAngle() { return mMaxAngle; }

void ParticleEmitter::spawnParticle() {
    Vector2 particlePos;
    if (mGameObject != nullptr) {
        particlePos = mGameObject->getTransform().position + mRelativeTransform.position;
    } else {
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

void ParticleEmitter::update() {
    if (mActive) {
        if (mEmitterMode == EmitterMode::Continuous) {
            static float amountOfParticlesToSpawn;
            amountOfParticlesToSpawn += mParticlesPerSecond * Time::deltaTime;
            int i = 0;
            for (i = 0; i < amountOfParticlesToSpawn; i++) {
                spawnParticle();
            }
            amountOfParticlesToSpawn -= i;
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

void ParticleEmitter::burst(int aAmount) {
    for (int i = 0; i < aAmount; i++) {
        spawnParticle();
    }
}

std::vector<Particle>& ParticleEmitter::getParticles() { return mParticles; }

void ParticleEmitter::setLayer(int aLayer) { mLayer = aLayer; }

int ParticleEmitter::getLayer() { return mLayer; }
