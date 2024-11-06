#include "Physics/World.h"
#include "BoxCollider.h"
#include "Vector2.h"
#include "box2d/collision.h"
#include "box2d/id.h"
#include "box2d/types.h"

World::World(Vector2 aGravity) {

    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = (b2Vec2){aGravity.x, aGravity.y};
    mWorldID = b2CreateWorld(&worldDef);
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
        bodyDef.type = b2_dynamicBody;
        break;
    }

    bodyDef.position = (b2Vec2){aBodyProxy.getPosition().x, aBodyProxy.getPosition().y};
    bodyDef.gravityScale = aBodyProxy.getGravityScale();
    // bodyDef.rotation = aBodyProxy.getRotation();
    bodyDef.fixedRotation = !aBodyProxy.getCanRotate();
    b2BodyId bodId = b2CreateBody(mWorldID, &bodyDef);

    for (BoxCollider* boxCollider : aBodyProxy.getBoxColliders()) {
        b2Polygon shape = b2MakeBox({boxCollider->getWidth(), boxCollider->getHeight()});
        b2_polygonShape shape = b2DefaultPolygonShape();
        shape.SetAsBox(boxCollider->getSize().x, boxCollider->getSize().y);

        b2FixtureDef fixtureDef = b2DefaultFixtureDef();
        fixtureDef.shape = shape;
        fixtureDef.density = aBodyProxy.getDensity();
        fixtureDef.friction = aBodyProxy.getFriction();
        fixtureDef.restitution = aBodyProxy.getRestitution();

        b2CreateFixture(mWorldID, boxCollider->getGameObjectID(), &fixtureDef);
    }
}

void World::updateBody(int aBodyID, BodyProxy& aBodyProxy) {}

void World::applyForce(int aBodyID, Vector2 aForce) {}

void World::setPosition(int aBodyID, Vector2 aPosition) {}

Vector2 World::getPosition() {}

void World::setGravity(Vector2 aGravity) {}

Vector2 World::getGravity() { return mGravity; }

std::vector<int> World::checkContactEvent() {}
