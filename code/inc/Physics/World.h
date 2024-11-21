#ifndef WORLD_H
#define WORLD_H

#include "Physics/BodyProxy.h"
#include "Vector2.h"
#include "box2d/box2d.h"
#include "box2d/id.h"
#include <utility>
#include <vector>
class World {
public:
    World();

    void executeWorldStep(float step, int subStep);

    void resetWorld();

    int createWorld(Vector2 aGravity);
    int createBody(BodyProxy& aBodyProxy);
    void createShape(BodyProxy& aBodyProxy, int aBodyID);
    void updateBody(int aBodyID, BodyProxy& aBodyProxy);
    void applyLinearForce(int aBodyID, std::vector<Vector2> aForce);
    void applyTorque(int aBodyID, std::vector<float> aTorque);

    void setPosition(int BodyID, Vector2 aPosition);
    Vector2 getPosition(int aBodyID);

    void setGravity(Vector2 aGravity);
    Vector2 getGravity();

    void updateBodyFlags(BodyProxy& aBodyProxy, int aBodyID);

    std::vector<std::pair<int, int>> getContactEvents();
    std::vector<std::pair<int, int>> getSensorEvents();

    void setBodyActivity(int aBodyID, bool aState);

private:
    b2WorldId mWorldID;
    Vector2 mGravity;
};

#endif // WORLD_H
