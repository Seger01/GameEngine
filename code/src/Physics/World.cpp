#include "Physics/World.h"
#include "box2d/box2d.h"

World::World() {}

int World::createWorld(Vector2 aGravity) {
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = (b2Vec2){aGravity.x, aGravity.y};
    mWorldID = b2CreateWorld(&worldDef);
    return 0;
}

void World::executeWorldStep(float aStep, int aSubStep) { b2World_Step(mWorldID, aStep, aSubStep); }

void World::resetWorld() { b2DestroyWorld(mWorldID); }

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
    bodyDef.fixedRotation = !aBodyProxy.getCanRotate();
    bodyDef.linearDamping = aBodyProxy.getLinearDamping();
    bodyDef.angularDamping = aBodyProxy.getAngularDamping();

    b2BodyId bodyID = b2CreateBody(mWorldID, &bodyDef);

    createShape(aBodyProxy, bodyID.index1);

    return bodyID.index1;
}

void World::createShape(BodyProxy& aBodyProxy, int aBodyID) {
    b2BodyId bodyID = {aBodyID, 0, 1};

    for (BoxCollider* boxCollider : aBodyProxy.getBoxColliders()) {
        b2Polygon polygon = b2MakeBox(boxCollider->getWidth(), boxCollider->getHeight());
        // std::cout << "BoxCollider width: " << boxCollider->getWidth() << std::endl;
        // std::cout << "BoxCollider height: " << boxCollider->getHeight() << std::endl;

        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.density = aBodyProxy.getDensity();
        shapeDef.friction = aBodyProxy.getFriction();
        shapeDef.restitution = aBodyProxy.getRestitution();
        shapeDef.isSensor = boxCollider->isTrigger();

        b2CreatePolygonShape(bodyID, &shapeDef, &polygon);
    }
}

void World::applyLinearForce(int aBodyID, std::vector<Vector2> aForce) {
    b2BodyId bodyID = {aBodyID, 0, 1};
    for (int i = 0; i < aForce.size(); i++) {
        b2Vec2 force = {aForce[i].x, aForce[i].y};
        b2Body_ApplyLinearImpulseToCenter(bodyID, force, true);
    }
}

void World::applyTorque(int aBodyID, std::vector<float> aTorque) {
    b2BodyId bodyID = {aBodyID, 0, 1};
    for (int torqueBufferIndex = 0; torqueBufferIndex < aTorque.size(); torqueBufferIndex++) {
        b2Body_ApplyTorque(bodyID, aTorque[torqueBufferIndex], true);
    }
}
void World::setPosition(int aBodyID, Vector2 aPosition) {
    b2BodyId bodyid = {aBodyID, 0, 1};
    std::cout << "Setting pos: (" << aPosition.x << ", " << aPosition.y << ")" << std::endl;
    b2Body_SetTransform(bodyid, {aPosition.x, aPosition.y}, {cos(0.0f), 0.0f});
}

Vector2 World::getPosition(int aBodyID) {
    b2BodyId bodyID = {aBodyID, 0, 1};
    Vector2 position = {b2Body_GetPosition(bodyID).x, b2Body_GetPosition(bodyID).y};
    return position;
}

void World::setGravity(Vector2 aGravity) {}

Vector2 World::getGravity() { return mGravity; }

std::vector<std::pair<int, int>> World::getContactEvents() {
    std::vector<std::pair<int, int>> collisionList;
    b2ContactEvents contactlist = b2World_GetContactEvents(mWorldID);

    for (int i = 0; i < contactlist.beginCount; i++) {
        collisionList.push_back({b2Shape_GetBody(contactlist.beginEvents[i].shapeIdA).index1,
                                 b2Shape_GetBody(contactlist.beginEvents[i].shapeIdB).index1});
    }
    return collisionList;
}

void World::setBodyActivity(int aBodyID, bool aState) {
    b2BodyId bodyID = {aBodyID, 0, 1};
    if (aState != b2Body_IsEnabled(bodyID)) {
        if (aState) {
            b2Body_Enable(bodyID);
        } else {
            b2Body_Disable(bodyID);
        }
    }
}

void World::updateBodyProperties(BodyProxy& aBodyProxy, int aBodyID) {
    std::cout << "Updating body properties" << std::endl;
    b2BodyId bodyID = {aBodyID, 0, 1};

    b2Body_SetFixedRotation(bodyID, !aBodyProxy.getCanRotate());
    b2Body_SetGravityScale(bodyID, aBodyProxy.getGravityScale());
    b2Body_SetLinearDamping(bodyID, aBodyProxy.getLinearDamping());
    b2Body_SetAngularDamping(bodyID, aBodyProxy.getAngularDamping());
}

void World::updateShapeProperties(BodyProxy& aBodyProxy, int aBodyID) {
    std::cout << "Updating shape properties" << std::endl;
    b2BodyId bodyID = {aBodyID, 0, 1};

    b2ShapeId shapeArray[aBodyProxy.getBoxColliders().size()];

    b2Body_GetShapes(bodyID, shapeArray, aBodyProxy.getBoxColliders().size());

    for (int i = 0; i < aBodyProxy.getBoxColliders().size(); i++) {
        b2Shape_SetDensity(shapeArray[i], aBodyProxy.getDensity());
        b2Shape_SetFriction(shapeArray[i], aBodyProxy.getFriction());
        b2Shape_SetRestitution(shapeArray[i], aBodyProxy.getRestitution());

        if (b2Shape_IsSensor(shapeArray[i]) != aBodyProxy.getBoxColliders().at(i)->isTrigger()) {
            b2DestroyShape(shapeArray[i]);
            // std::cout << aBodyProxy.getBoxColliders().at(i)->isTrigger() << std::endl;
            createShape(aBodyProxy, aBodyID);
        }
    }
}

std::vector<std::pair<int, int>> World::getSensorEvents() {
    std::vector<std::pair<int, int>> sensorList;
    b2SensorEvents sensorEvents = b2World_GetSensorEvents(mWorldID);

    for (int i = 0; i < sensorEvents.beginCount; i++) {
        // std::cout << "sensor found: " << "(" << sensorEvents.beginEvents[i].sensorShapeId.index1 << ","
        //           << sensorEvents.beginEvents[i].visitorShapeId.index1 << ")" << std::endl;
        sensorList.push_back({b2Shape_GetBody(sensorEvents.beginEvents[i].sensorShapeId).index1,
                              b2Shape_GetBody(sensorEvents.beginEvents[i].visitorShapeId).index1});
    }

    return sensorList;
}
