#include "Physics/World.h"

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
    std::cout << "creating world bodies" << std::endl;
    b2BodyDef bodyDef = b2DefaultBodyDef();

    switch (aBodyProxy.getBodyType()) {
    case BodyType::STATIC:
        bodyDef.type = b2_staticBody;
        break;
    case BodyType::DYNAMIC:
        bodyDef.type = b2_dynamicBody;
        break;
    }

    bodyDef.position = (b2Vec2){aBodyProxy.getPosition().x, aBodyProxy.getPosition().y};
    bodyDef.gravityScale = aBodyProxy.getGravityScale();
    // bodyDef.rotation = aBodyProxy.getRotation();
    bodyDef.fixedRotation = !aBodyProxy.getCanRotate();
    b2BodyId bodyID = b2CreateBody(mWorldID, &bodyDef);
    std::cout << bodyID.revision << std::endl;

    for (BoxCollider* boxCollider : aBodyProxy.getBoxColliders()) {
        b2Polygon polygon = b2MakeBox(boxCollider->getWidth(), boxCollider->getHeight());

        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.density = aBodyProxy.getDensity();
        shapeDef.friction = aBodyProxy.getFriction();
        shapeDef.restitution = aBodyProxy.getRestitution();

        b2CreatePolygonShape(bodyID, &shapeDef, &polygon);
    }
    return 0;
}

void World::updateBody(int aBodyID, BodyProxy& aBodyProxy) {}

void World::applyForce(int aBodyID, Vector2 aForce) {}

void World::setPosition(int aBodyID, Vector2 aPosition) {}

Vector2 World::getPosition() {}

void World::setGravity(Vector2 aGravity) {}

Vector2 World::getGravity() { return mGravity; }

std::vector<int> World::checkContactEvent() {}
