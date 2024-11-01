#ifndef PARTICLEEMITTER_H
#define PARTICLEEMITTER_H

#include <vector>

#include "Component.h"
#include "Particle.h"

enum EmitterMode {
    Continuous,
    Burst,
};

class ParticleEmitter : public Component {
public:
    ParticleEmitter(EmitterMode aEmitterMode, float aSpeed, float aAcceleration, int aMinLifeTimeMs, int aMaxLifeTimeMs,
                    Vector2 aSize, Vector2 aSizeShift, float aRotation, float angularVelocity,
                    float angularAcceleration, std::vector<Color> aColorGradient);

    ~ParticleEmitter();

    void update();

    void setRelativeTransform(Transform aTransform);
    Transform& getRelativeTransform();

    void setParticlesPerSecond(int aParticlesPerSecond);
    void setAngle(int aMinAngle, int aMaxAngle);

    std::vector<Particle>& getParticles();

private:
    void spawnParticle();

    Vector2 generateRandomVelocity(float minSpeed, float maxSpeed, int minAngle, int maxAngle);

private:
    Transform relativeTransform;

    int particlesPerSecond;
    std::vector<Particle> particles;

    EmitterMode emitterMode;

    int minAngle;
    int maxAngle;

    int minLifeTimeMs;
    int maxLifeTimeMs;

    float velocity;
    float acceleration;
    Vector2 size;
    Vector2 sizeShift;
    float rotation;
    float angularVelocity;
    float angularAcceleration;
    std::vector<Color> colorGradient;
};

#endif // PARTICLEEMITTER_H
