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
                    Vector2 aSize, Vector2 aEndSize, float aRotation, float angularVelocity, float angularAcceleration,
                    std::vector<Color> aColorGradient);

    ~ParticleEmitter();

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
    Vector2 mEndSize;
    float mRotation;
    float mAngularVelocity;
    float mAngularAcceleration;
    std::vector<Color> mColorGradient;

    int mLayer = 0;
};

#endif // PARTICLEEMITTER_H
