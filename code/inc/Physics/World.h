#ifndef WORLD_H
#define WORLD_H

#include "Physics/BodyProxy.h"
#include "Vector2.h"
#include "box2d/box2d.h"
#include <utility>
#include <vector>

class World {
public:
    World();

    int createWorld(Vector2 aGravity);
    void resetWorld();

    void executeWorldStep(float step, int subStep);

    void updateBodyPosition(int aBodyID, BodyProxy& aBodyProxy);
    void updateBodyProperties(BodyProxy& aBodyProxy, int aBodyID);
    void updateShapeProperties(BodyProxy& aBodyProxy, int aBodyID);

    int createBody(BodyProxy& aBodyProxy);
    void createShape(BodyProxy& aBodyProxy, int aBodyID);

    void applyLinearForce(int aBodyID, std::vector<Vector2> aForce);
    void applyTorque(int aBodyID, std::vector<float> aTorque);

    void setPosition(int BodyID, Vector2 aPosition);
    void setGravity(Vector2 aGravity);
    std::vector<std::pair<int, int>> getContactEvents();
    std::vector<std::pair<int, int>> getSensorEvents();

    Vector2 getPosition(int aBodyID);
    Vector2 getGravity();

    void setBodyActivity(int aBodyID, bool aState);

private:
    b2WorldId mWorldID;
    Vector2 mGravity;
};

#endif // WORLD_H
