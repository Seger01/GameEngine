#include "ParticleEmitter.h"

#include <cmath>
#include <cstdlib>
#include <iostream>

#include "GameObject.h"
#include "Time.h"

ParticleEmitter::ParticleEmitter(EmitterMode aEmitterMode, float aSpeed, float aAcceleration, int aMinLifeTimeMs,
                                 int aMaxLifeTimeMs, Vector2 aSize, Vector2 aSizeShift, float aRotation,
                                 float angularVelocity, float angularAcceleration, std::vector<Color> aColorGradient)
    : emitterMode(aEmitterMode), velocity(aSpeed), acceleration(aAcceleration), minLifeTimeMs(aMinLifeTimeMs),
      maxLifeTimeMs(aMaxLifeTimeMs), size(aSize), sizeShift(aSizeShift), rotation(aRotation),
      angularVelocity(angularVelocity), angularAcceleration(angularAcceleration), colorGradient(aColorGradient) {
    particlesPerSecond = 0;
    minAngle = 0;
    maxAngle = 0;
}

ParticleEmitter::~ParticleEmitter() { particles.resize(0); }

Vector2 ParticleEmitter::generateRandomVelocity(float minSpeed, float maxSpeed, int minAngle, int maxAngle) {
    // Generate a random angle between minAngle and maxAngle
    float angle = (minAngle + (static_cast<float>(rand()) / RAND_MAX * (maxAngle - minAngle))) * (M_PI / 180.0f);
    angle = angle + relativeTransform.rotation;

    // Generate a random speed between minSpeed and maxSpeed
    float speed = minSpeed + static_cast<float>(rand()) / RAND_MAX * (maxSpeed - minSpeed);

    // Calculate velocity components
    Vector2 velocity;
    velocity.x = std::cos(angle) * speed;
    velocity.y = std::sin(angle) * speed;

    return velocity;
}

void ParticleEmitter::setRelativeTransform(Transform aTransform) { relativeTransform = aTransform; }

Transform& ParticleEmitter::getRelativeTransform() { return relativeTransform; }

void ParticleEmitter::setParticlesPerSecond(int aParticlesPerSecond) { particlesPerSecond = aParticlesPerSecond; }

void ParticleEmitter::setAngle(int aMinAngle, int aMaxAngle) {
    minAngle = aMinAngle;
    maxAngle = aMaxAngle;

    if (minAngle > maxAngle) {
        std::swap(minAngle, maxAngle);
    }

    if (maxAngle - minAngle > 360) {
        maxAngle = 0;
        maxAngle = minAngle + 360;
    }
}

void ParticleEmitter::spawnParticle() {
    // Particle particle(Vector2(0, 0), Vector2(0, 0), acceleration, lifeTime, size, rotation, angularVelocity,
    //                   angularAcceleration, colorGradient[0]);

    Vector2 particlePos = mGameObject->getTransform().position + relativeTransform.position;
    Vector2 particleVel = generateRandomVelocity(velocity, velocity, minAngle, maxAngle);
    float particleAccel = acceleration;

    int particleLifeTime = minLifeTimeMs + (rand() % (maxLifeTimeMs - minLifeTimeMs + 1));

    Vector2 particleSize = size;
    Vector2 particleSizeShift = sizeShift;
    float particleRotation = rotation;
    float particleAngularVelocity = angularVelocity;
    float particleAngularAcceleration = angularAcceleration;
    std::vector<Color> particleColorGradient = colorGradient;

    Particle particle(particlePos, particleVel, particleAccel, particleLifeTime, maxLifeTimeMs, particleSize,
                      particleSizeShift, particleRotation, particleAngularVelocity, particleAngularAcceleration,
                      particleColorGradient);

    particles.push_back(particle);
}

void ParticleEmitter::update() {
    if (emitterMode == EmitterMode::Continuous) {
        int amountOfParticlesToSpawn = (particlesPerSecond * Time::deltaTime) / 1000;
        for (int i = 0; i < amountOfParticlesToSpawn; i++) {
            spawnParticle();
        }
    }

    std::cout << "Amount of particles: " << particles.size() << std::endl;

    // for (auto& particle : particles) {
    //     particle.update();
    //     if (particle.getLifeTime() <= 0) {
    //         particles.erase(particles.begin());
    //     }
    // }
    for (int i = 0; i < particles.size();) { // Note: no increment here
        particles[i].update();
        if (particles[i].getLifeTime() <= 0) {
            particles.erase(particles.begin() + i); // Erase the element at index i
        } else {
            ++i; // Only increment if no element was erased
        }
    }
}

std::vector<Particle>& ParticleEmitter::getParticles() { return particles; }
