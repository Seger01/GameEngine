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

    Vector2 generateRandomVelocity(float aMinSpeed, float aMaxSpeed, int aMinAngle, int aMaxAngle);

private:
    Transform mRelativeTransform;

    int mParticlesPerSecond;
    std::vector<Particle> mParticles;

    EmitterMode mEmitterMode;

    int mMinAngle;
    int mMaxAngle;

    int mMinLifeTimeMs;
    int mMaxLifeTimeMs;

    float mVelocity;
    float mAcceleration;
    Vector2 mSize;
    Vector2 mSizeShift;
    float mRotation;
    float mAngularVelocity;
    float mAngularAcceleration;
    std::vector<Color> mColorGradient;
};

#endif // PARTICLEEMITTER_H
