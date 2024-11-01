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
             Vector2 aSizeShift, float aRotation, float angularVelocity, float angularAcceleration,
             std::vector<Color> aColorGradient);
    ~Particle();

    void update();

    // void setVelocity(Vector2 aVelocity);
    // void setAcceleration(Vector2 aAcceleration);
    // void setLifeTime(float aLifeTime);
    // void setScale(Vector2 aScale);
    // void setRotation(float rotation);
    // void setAngularVelocity(float angularVelocity);
    // void setAngularAcceleration(float angularAcceleration);
    // void setColor(Color aColor);

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
    int lifeTimeRemaining;
    int initialLifeTime;

    Vector2 size;
    Vector2 sizeShift;
    float rotation;
    float angularVelocity;
    float angularAcceleration;
    std::vector<Color> colorGradient;

    bool interpolateColor = true;
};

#endif // PARTICLE_H
