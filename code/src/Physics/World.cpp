#include "Physics/World.h"
#include "box2d/box2d.h"
#include "box2d/math_functions.h"
#include <cmath>
#include <math.h>

World::World() {}

World::World(Vector2 aGravity)
{
	b2WorldDef worldDef = b2DefaultWorldDef();
	mGravity = aGravity;
	worldDef.gravity = (b2Vec2){mGravity.x, mGravity.y};
	mWorldID = b2CreateWorld(&worldDef);
}

int World::createWorld(Vector2 aGravity) { return 0; }

World::~World()
{ // b2DestroyWorld(mWorldID);
}

void World::executeWorldStep(float aStep, int aSubStep) { b2World_Step(mWorldID, aStep, aSubStep); }

void World::resetWorld()
{
	b2DestroyWorld(mWorldID);
	b2WorldDef worldDef = b2DefaultWorldDef();
	worldDef.gravity = (b2Vec2){mGravity.x, mGravity.y};
	mWorldID = b2CreateWorld(&worldDef);
}

BodyID World::createBody(BodyProxy& aBodyProxy)
{

	b2BodyDef bodyDef = b2DefaultBodyDef();

	switch (aBodyProxy.getBodyType())
	{
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
	createShape(aBodyProxy, {bodyID.index1, bodyID.revision, bodyID.world0});

	BodyID convertedBodyID = {bodyID.index1, bodyID.revision, bodyID.world0};
	return convertedBodyID;
}

void World::applyRotationalImpusle(std::vector<float> aTorque, BodyProxy& aBodyProxy, float aImpulse, BodyID aBodyID) {}

void World::createShape(BodyProxy& aBodyProxy, BodyID aBodyID)
{

	b2BodyId bodyID = convertToB2BodyID(aBodyID);

	for (BoxCollider* boxCollider : aBodyProxy.getBoxColliders())
	{
		b2Polygon polygon =
			b2MakeOffsetBox(boxCollider->getWidth(), boxCollider->getHeight(),
							{boxCollider->getTransform().position.x, boxCollider->getTransform().position.y},
							boxCollider->getRotation());
		b2ShapeDef shapeDef = b2DefaultShapeDef();
		shapeDef.density = aBodyProxy.getDensity();
		shapeDef.friction = aBodyProxy.getFriction();
		shapeDef.restitution = aBodyProxy.getRestitution();
		shapeDef.isSensor = boxCollider->isTrigger();

		uint16_t maskBits = 0;
		for (int category : boxCollider->getCollideWithCategory())
		{
			maskBits |= (1 << category); // Generate the bitmask
		}
		shapeDef.filter.categoryBits = (1 << boxCollider->getCollideCategory());

		shapeDef.filter.maskBits = maskBits;
		b2CreatePolygonShape(bodyID, &shapeDef, &polygon);
	}

	for (CircleCollider* circleCollider : aBodyProxy.getCircleColliders())
	{
		b2Circle circle;
		circleCollider->getRadius();

		circle.radius = circleCollider->getRadius();
		circle.center = {circleCollider->getTransform().position.x, circleCollider->getTransform().position.y};

		b2ShapeDef shapeDef = b2DefaultShapeDef();
		shapeDef.density = aBodyProxy.getDensity();
		shapeDef.friction = aBodyProxy.getFriction();
		shapeDef.restitution = aBodyProxy.getRestitution();
		shapeDef.isSensor = circleCollider->getIsTrigger();

		uint16_t maskBits = 0;
		for (int category : circleCollider->getCollideWithCategory())
		{
			maskBits |= (1 << category); // Generate the bitmask
		}
		shapeDef.filter.categoryBits = (1 << circleCollider->getCollideCategory());

		shapeDef.filter.maskBits = maskBits;
		b2CreateCircleShape(bodyID, &shapeDef, &circle);
	};
}

void World::deleteBody(BodyID aBodyID)
{
	b2BodyId bodyID = convertToB2BodyID(aBodyID);
	if (b2Body_IsValid(bodyID))
	{
		b2DestroyBody(bodyID);
	}
}

void World::scaleShape(BodyProxy& aBodyProxy, BodyID aBodyID, float aScale)
{
	b2BodyId bodyID = convertToB2BodyID(aBodyID);

	for (int i = 0; i < aBodyProxy.getBoxColliders().size(); i++)
	{
		BoxCollider* boxCollider = aBodyProxy.getBoxColliders().at(i);
	}
}

void World::applyLinearForce(std::vector<Vector2> aForce, BodyID aBodyID)
{
	b2BodyId bodyID = convertToB2BodyID(aBodyID);
	for (int i = 0; i < aForce.size(); i++)
	{
		b2Vec2 force = {aForce[i].x, aForce[i].y};
		b2Body_ApplyLinearImpulseToCenter(bodyID, force, true);
	}
}

void World::applyTorque(std::vector<float> aTorque, BodyID aBodyID)
{
	b2BodyId bodyID = convertToB2BodyID(aBodyID);
	for (int torqueBufferIndex = 0; torqueBufferIndex < aTorque.size(); torqueBufferIndex++)
	{
		b2Body_ApplyTorque(bodyID, aTorque[torqueBufferIndex], true);
	}
}

void World::applyLinearImpulse(std::vector<Vector2> aImpulseLeft, std::vector<Vector2> aImpulseRight, float aWidth,
							   float aHeight, BodyID aBodyID)
{
	b2BodyId bodyID = convertToB2BodyID(aBodyID);
	for (int i = 0; i < aImpulseLeft.size(); i++)
	{
		b2Vec2 impulseLeft = {aImpulseLeft[i].x, aImpulseLeft[i].y};
		b2Vec2 impulseRight = {aImpulseRight[i].x, aImpulseRight[i].y};

		b2Vec2 pos = b2Body_GetPosition(bodyID);
		b2Vec2 midLeft = {pos.x, pos.y + aHeight};
		b2Vec2 midRight = {pos.x + aWidth, pos.y + aHeight};

		b2Body_ApplyLinearImpulse(bodyID, impulseLeft, midLeft, true);
		b2Body_ApplyLinearImpulse(bodyID, impulseRight, midRight, true);
	}
}

void World::setPosition(Vector2 aPosition, float aRotation, BodyID aBodyID)
{
	b2BodyId bodyid = convertToB2BodyID(aBodyID);
	float radians = aRotation * (M_PI / 180.0f);
	b2Rot rot;
	rot.s = sin(radians);
	rot.c = cos(radians);

	b2Body_SetTransform(bodyid, {aPosition.x, aPosition.y}, rot);
}

Vector2 World::getPosition(BodyID aBodyID)
{
	b2BodyId bodyID = convertToB2BodyID(aBodyID);
	Vector2 position = {b2Body_GetPosition(bodyID).x, b2Body_GetPosition(bodyID).y};
	return position;
}

float World::getRotation(BodyID aBodyID)
{
	b2BodyId bodyID = convertToB2BodyID(aBodyID);

	if (b2Body_IsValid(bodyID))
	{
		float radians = atan2(b2Body_GetRotation(bodyID).s, b2Body_GetRotation(bodyID).c);
		return radians * (180.0f / M_PI);
	}
	return 0;
}

void World::setGravity(Vector2 aGravity)
{
	mGravity = aGravity;
	b2World_SetGravity(mWorldID, {aGravity.x, aGravity.y});
}

Vector2 World::getGravity() { return mGravity; }

std::vector<std::pair<int, int>> World::getContactEvents()
{
	std::vector<std::pair<int, int>> collisionList;
	b2ContactEvents contactlist = b2World_GetContactEvents(mWorldID);

	for (int i = 0; i < contactlist.beginCount; i++)
	{
		if (b2Shape_IsValid(contactlist.beginEvents[i].shapeIdA) &&
			b2Shape_IsValid(contactlist.beginEvents[i].shapeIdB))
		{
			collisionList.push_back({b2Shape_GetBody(contactlist.beginEvents[i].shapeIdA).index1,
									 b2Shape_GetBody(contactlist.beginEvents[i].shapeIdB).index1});
		}
	}
	return collisionList;
}

void World::setBodyActivity(bool aState, BodyID aBodyID)
{
	b2BodyId bodyID = convertToB2BodyID(aBodyID);
	if (b2Body_IsValid(bodyID))
	{
		if (aState)
		{
			b2Body_Enable(bodyID);
		}
		else
		{
			b2Body_Disable(bodyID);
		}
	}
}

void World::updateBodyProperties(BodyProxy& aBodyProxy, BodyID aBodyID)
{
	b2BodyId bodyID = convertToB2BodyID(aBodyID);

	b2Body_SetFixedRotation(bodyID, !aBodyProxy.getCanRotate());
	b2Body_SetGravityScale(bodyID, aBodyProxy.getGravityScale());
	b2Body_SetLinearDamping(bodyID, aBodyProxy.getLinearDamping());
	b2Body_SetAngularDamping(bodyID, aBodyProxy.getAngularDamping());
}

void World::updateShapeProperties(BodyProxy& aBodyProxy, BodyID aBodyID)
{
	b2BodyId bodyID = convertToB2BodyID(aBodyID);
	int size = aBodyProxy.getBoxColliders().size() + aBodyProxy.getCircleColliders().size();

	b2ShapeId shapeArray[size];

	b2Body_GetShapes(bodyID, shapeArray, size);
	int boxcounter = 0;
	int circlecounter = 0;
	for (int i = 0; i < size; i++)
	{
		b2ShapeType shapeType = b2Shape_GetType(shapeArray[i]);
		if (shapeType == b2_polygonShape)
		{
			if (aBodyProxy.getBoxColliders().at(boxcounter) != nullptr)
			{
				BoxCollider* tempBoxCollider = aBodyProxy.getBoxColliders().at(boxcounter);
				b2Shape_SetDensity(shapeArray[i], aBodyProxy.getDensity());
				b2Shape_SetFriction(shapeArray[i], aBodyProxy.getFriction());
				b2Shape_SetRestitution(shapeArray[i], aBodyProxy.getRestitution());

				if (b2Shape_IsSensor(shapeArray[i]) != tempBoxCollider->isTrigger())
				{
					b2DestroyShape(shapeArray[i]);
					createShape(aBodyProxy, aBodyID);
				}

				uint16_t maskBits = 0;
				for (int category : tempBoxCollider->getCollideWithCategory())
				{
					maskBits |= (1 << category); // Generate the bitmask
				}

				b2Filter tempFilter = b2Shape_GetFilter(shapeArray[i]);
				tempFilter.maskBits = maskBits;
				tempFilter.categoryBits = (1 << tempBoxCollider->getCollideCategory());

				b2Shape_SetFilter(shapeArray[i], tempFilter);
				boxcounter++;
			}
		}
		else if (shapeType == b2_circleShape)
		{
			if (aBodyProxy.getCircleColliders().at(i) != nullptr)
			{
				CircleCollider* tempCircleCollider = aBodyProxy.getCircleColliders().at(circlecounter);
				b2Shape_SetDensity(shapeArray[i], aBodyProxy.getDensity());
				b2Shape_SetFriction(shapeArray[i], aBodyProxy.getFriction());
				b2Shape_SetRestitution(shapeArray[i], aBodyProxy.getRestitution());

				if (b2Shape_IsSensor(shapeArray[i]) != tempCircleCollider->getIsTrigger())
				{
					b2DestroyShape(shapeArray[i]);
					createShape(aBodyProxy, aBodyID);
				}

				uint16_t maskBits = 0;
				for (int category : tempCircleCollider->getCollideWithCategory())
				{
					maskBits |= (1 << category); // Generate the bitmask
				}

				b2Filter tempFilter = b2Shape_GetFilter(shapeArray[i]);
				tempFilter.maskBits = maskBits;
				tempFilter.categoryBits = (1 << tempCircleCollider->getCollideCategory());

				b2Shape_SetFilter(shapeArray[i], tempFilter);
				circlecounter++;
			}
		}
	}
}

std::vector<std::pair<int, int>> World::getSensorEvents()
{
	std::vector<std::pair<int, int>> sensorList;
	b2SensorEvents sensorEvents = b2World_GetSensorEvents(mWorldID);

	for (int i = 0; i < sensorEvents.beginCount; i++)
	{
		if (b2Shape_IsValid(sensorEvents.beginEvents[i].sensorShapeId) &&
			b2Shape_IsValid(sensorEvents.beginEvents[i].visitorShapeId))
		{
			sensorList.push_back({b2Shape_GetBody(sensorEvents.beginEvents[i].sensorShapeId).index1,
								  b2Shape_GetBody(sensorEvents.beginEvents[i].visitorShapeId).index1});
		}
	}

	return sensorList;
}

b2BodyId World::convertToB2BodyID(BodyID aBodyID)
{
	b2BodyId bodyID;
	bodyID.index1 = aBodyID.bodyID;
	bodyID.revision = aBodyID.revision;
	bodyID.world0 = aBodyID.world0;
	return bodyID;
}
