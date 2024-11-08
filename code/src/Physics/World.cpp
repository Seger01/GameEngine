#include "Physics/World.h"
#include "box2d/box2d.h"
#include "box2d/id.h"
#include "box2d/math_functions.h"
#include "box2d/types.h"

World::World() {}

int World::createWorld(Vector2 aGravity) {
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = (b2Vec2){aGravity.x, aGravity.y};
    mWorldID = b2CreateWorld(&worldDef);
    return 0;
}

void World::executeWorldStep(float aStep, int aSubStep) { b2World_Step(mWorldID, aStep, aSubStep); }

void World::reset() {}

int World::createBody(BodyProxy& aBodyProxy) {
    b2BodyDef bodyDef = b2DefaultBodyDef();

    switch (aBodyProxy.getBodyType()) {
    case BodyType::STATIC:

        bodyDef.type = b2_staticBody;

        break;
    case BodyType::DYNAMIC:
        std::cout << "create dynamicbody" << std::endl;
        bodyDef.type = b2_dynamicBody;
        break;
    }

    bodyDef.position = (b2Vec2){aBodyProxy.getPosition().x, aBodyProxy.getPosition().y};
    bodyDef.gravityScale = aBodyProxy.getGravityScale();
    bodyDef.fixedRotation = !aBodyProxy.getCanRotate();
    b2BodyId bodyID = b2CreateBody(mWorldID, &bodyDef);
    std::cout << "creating body at: (" << aBodyProxy.getPosition().x << ", " << aBodyProxy.getPosition().y << ")"
              << std::endl;
    for (BoxCollider* boxCollider : aBodyProxy.getBoxColliders()) {
        b2Polygon polygon = b2MakeBox(1000, 1000);

        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.density = aBodyProxy.getDensity();
        shapeDef.friction = aBodyProxy.getFriction();
        shapeDef.restitution = aBodyProxy.getRestitution();

        b2CreatePolygonShape(bodyID, &shapeDef, &polygon);
        b2Body_EnableSleep(bodyID, true);

        std::cout << "creating box collider at: (" << boxCollider->getTransform().position.x << ", "
                  << boxCollider->getTransform().position.y << ")" << std::endl;
    }
    return bodyID.index1;
}

void World::updateBody(int aBodyID, BodyProxy& aBodyProxy) {
    b2BodyId test = {aBodyID, 0, 1};
    b2Body_SetTransform(test, {aBodyProxy.getPosition().x, aBodyProxy.getPosition().y}, b2Body_GetRotation(test));
    std::cout << "updating body id: " << aBodyID << std::endl;
    std::cout << "Position of ID: " << test.index1 << "is: (" << b2Body_GetPosition(test).x << ", "
              << b2Body_GetPosition(test).y << ")" << std::endl;
}

void World::applyForce(int aBodyID, Vector2 aForce) {}

void World::setPosition(int aBodyID, Vector2 aPosition) {}

Vector2 World::getPosition() {}

void World::setGravity(Vector2 aGravity) {}

Vector2 World::getGravity() { return mGravity; }

std::vector<int> World::checkContactEvent(int aBodyID) {
    b2BodyId test2 = {1, 0, 1};
    b2Body_ApplyLinearImpulseToCenter(test2, {5000.0f, 0.0f}, true);

    b2Body_SetAwake(test2, true);
    b2ContactData contactData[10];
    int test = b2Body_GetContactData(test2, contactData, 1);
    std::cout << test << std::endl;
    if (test != 0) {
        while (true)
            ;
    }
    return std::vector<int>(1);
}
