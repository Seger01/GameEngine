#ifndef WORLD_H
#define WORLD_H

#include "Physics/BodyProxy.h"
#include "RigidBody.h"
#include "Vector2.h"
#include "box2d/box2d.h"
#include "box2d/id.h"
#include <utility>
#include <vector>

class World {
public:
    World(Vector2 aGravity);
    World();
    ~World();

    int createWorld(Vector2 aGravity);
    void resetWorld();

    void executeWorldStep(float step, int subStep);

    void updateBodyPosition(BodyProxy& aBodyProxy, BodyID aBodyID);
    void updateBodyProperties(BodyProxy& aBodyProxy, BodyID aBodyID);
    void updateShapeProperties(BodyProxy& aBodyProxy, BodyID aBodyID);

    BodyID createBody(BodyProxy& aBodyProxy);
    void createShape(BodyProxy& aBodyProxy, BodyID aBodyID);

    void deleteBody(BodyID aBodyID);

    void applyLinearForce(std::vector<Vector2> aForce, BodyID aBodyID);
    void applyTorque(std::vector<float> aTorque, BodyID aBodyID);

    void setPosition(Vector2 aPosition, BodyID aBodyID);
    void setGravity(Vector2 aGravity);
    std::vector<std::pair<int, int>> getContactEvents();
    std::vector<std::pair<int, int>> getSensorEvents();

    Vector2 getPosition(BodyID aBodyID);
    Vector2 getGravity();

    void setBodyActivity(bool aState, BodyID aBodyID);

    b2BodyId convertToB2BodyID(BodyID aBodyID);

private:
    b2WorldId mWorldID;
    Vector2 mGravity;
};

#endif // WORLD_H
