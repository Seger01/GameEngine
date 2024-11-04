#include "Physics/World.h"
#include "Vector2.h"

World::World(Vector2 aGravity) {

    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = (b2Vec2){aGravity.x, aGravity.y};
    mWorldID = b2CreateWorld(&worldDef);
}

void World::executeWorldStep(float aStep, int aSubStep) {}

void World::reset() {}

int World::createBody(BodyProxy& aBodyProxy) {}

void World::updateBody(int aBodyID, BodyProxy& aBodyProxy) {}

void World::applyForce(int aBodyID, Vector2 aForce) {}

void World::setPosition(int aBodyID, Vector2 aPosition) {}

Vector2 World::getPosition() {}

void World::setGravity(Vector2 aGravity) {}

Vector2 World::getGravity() {}

std::vector<int> World::checkContactEvent() {}
