#ifndef PARTICLE_H
#define PARTICLE_H

#include <vector>

#include "Global/Color.h"
#include "Transform.h"
#include "Vector2.h"

class Particle {
public:
    Particle();
    Particle(Vector2 aPosition, Vector2 aVelocity, float aAcceleration, int aLifeTime, int aMaxLifeTime, Vector2 aSize,
             Vector2 aEndSize, float aRotation, float angularVelocity, float angularAcceleration,
             std::vector<Color> aColorGradient);
    ~Particle();

    void update();

    Vector2 getPosition();
    float getLifeTime();
    Vector2 getSize();
    float getRotation();
    Color getColor();

private:
    Color calculateColor();
    Color calculateInterpolatedColor();
    Color getNearestColor();

private:
    Vector2 position;
    Vector2 velocity;
    float acceleration;

    int maxLifeTime;
    float lifeTimeRemainingSec;
    int initialLifeTime;

    Vector2 startSize;
    Vector2 size;
    Vector2 endSize;
    float rotation;
    float angularVelocity;
    float angularAcceleration;
    std::vector<Color> colorGradient;

    bool interpolateColor = true;
};

#endif // PARTICLE_H
