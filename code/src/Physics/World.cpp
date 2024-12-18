#include "Physics/World.h"
#include "box2d/box2d.h"
#include "box2d/collision.h"

World::World() {}

World::World(const Vector2& aGravity)
{
	b2WorldDef worldDef = b2DefaultWorldDef();
	mGravity = aGravity;
	worldDef.gravity = (b2Vec2){mGravity.x, mGravity.y};
	mWorldID = b2CreateWorld(&worldDef);
}

/**
 * @brief Executes a world step
 * @param aStep step rate of worldstep
 * @param aSubStep substep rate of worldstep
 */
void World::executeWorldStep(float aStep, int aSubStep) { b2World_Step(mWorldID, aStep, aSubStep); }

/**
 * @brief Creates a body in the box2d world
 * @param aBodyProxy The body proxy containg body data
 * @return The body ID
 */
BodyID World::createBody(const BodyProxy& aBodyProxy)
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
	float radians = aBodyProxy.getRotation() * (M_PI / 180.0f);
	b2Rot rot;
	rot.s = sin(radians);
	rot.c = cos(radians);
	bodyDef.rotation = rot;
	bodyDef.gravityScale = aBodyProxy.getGravityScale();
	bodyDef.fixedRotation = !aBodyProxy.getCanRotate();
	bodyDef.linearDamping = aBodyProxy.getLinearDamping();
	bodyDef.angularDamping = aBodyProxy.getAngularDamping();
	bodyDef.allowFastRotation = true;
	bodyDef.isAwake = true;
	b2BodyId bodyID = b2CreateBody(mWorldID, &bodyDef);
	createShape(aBodyProxy, {bodyID.index1, bodyID.revision, bodyID.world0});

	BodyID convertedBodyID = {bodyID.index1, bodyID.revision, bodyID.world0};
	return convertedBodyID;
}

/**
 * @brief Creates a shape in a body
 * @param aBodyProxy The body proxy containing the shape data
 * @param aBodyID The body ID which the shape is created in
 */
void World::createShape(const BodyProxy& aBodyProxy, const BodyID& aBodyID)
{

	b2BodyId bodyID = convertToB2BodyID(aBodyID);

	for (BoxCollider& boxCollider : aBodyProxy.getBoxColliders())
	{
		b2Polygon polygon = b2MakeOffsetBox(
			boxCollider.getWidth(), boxCollider.getHeight(),
			{boxCollider.getTransform().position.x, boxCollider.getTransform().position.y}, boxCollider.getRotation());
		b2ShapeDef shapeDef = b2DefaultShapeDef();
		shapeDef.density = aBodyProxy.getDensity();
		shapeDef.friction = aBodyProxy.getFriction();
		shapeDef.restitution = aBodyProxy.getRestitution();
		shapeDef.isSensor = boxCollider.isTrigger();

		uint16_t maskBits = 0;
		for (int category : boxCollider.getCollideWithCategory())
		{
			maskBits |= (1 << category); // Generate the bitmask
		}
		shapeDef.filter.categoryBits = (1 << boxCollider.getCollideCategory());

		shapeDef.filter.maskBits = maskBits;
		b2CreatePolygonShape(bodyID, &shapeDef, &polygon);
	}

	for (CircleCollider& circleCollider : aBodyProxy.getCircleColliders())
	{
		b2Circle circle;
		circleCollider.getRadius();

		circle.radius = circleCollider.getRadius();
		circle.center = {circleCollider.getTransform().position.x, circleCollider.getTransform().position.y};

		b2ShapeDef shapeDef = b2DefaultShapeDef();
		shapeDef.density = aBodyProxy.getDensity();
		shapeDef.friction = aBodyProxy.getFriction();
		shapeDef.restitution = aBodyProxy.getRestitution();
		shapeDef.forceContactCreation = true;
		shapeDef.enablePreSolveEvents = true;
		shapeDef.isSensor = circleCollider.getIsTrigger();

		uint16_t maskBits = 0;
		for (int category : circleCollider.getCollideWithCategory())
		{
			maskBits |= (1 << category);
		}
		shapeDef.filter.categoryBits = (1 << circleCollider.getCollideCategory());

		shapeDef.filter.maskBits = maskBits;
		b2CreateCircleShape(bodyID, &shapeDef, &circle);
	};
}

/**
 * @brief Updates the properties of a body
 * @param aBodyProxy The body proxy containing the new position
 * @param aBodyID The body ID to update
 */
void World::updateBodyProperties(const BodyProxy& aBodyProxy, const BodyID& aBodyID)
{
	b2BodyId bodyID = convertToB2BodyID(aBodyID);

	b2Body_SetFixedRotation(bodyID, !aBodyProxy.getCanRotate());
	b2Body_SetGravityScale(bodyID, aBodyProxy.getGravityScale());
	b2Body_SetLinearDamping(bodyID, aBodyProxy.getLinearDamping());
	b2Body_SetAngularDamping(bodyID, aBodyProxy.getAngularDamping());
}

/**
 * @brief Updates properties of a shape
 * @param aBodyProxy The shape flags
 * @param aBodyID The body ID of the body containing the shapes
 */
void World::updateShapeProperties(const BodyProxy& aBodyProxy, const BodyID& aBodyID)
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
			try
			{

				BoxCollider& tempBoxCollider = aBodyProxy.getBoxColliders().at(boxcounter);
				b2Shape_SetDensity(shapeArray[i], aBodyProxy.getDensity());
				b2Shape_SetFriction(shapeArray[i], aBodyProxy.getFriction());
				b2Shape_SetRestitution(shapeArray[i], aBodyProxy.getRestitution());

				if (b2Shape_IsSensor(shapeArray[i]) != tempBoxCollider.isTrigger())
				{
					b2DestroyShape(shapeArray[i]);
					createShape(aBodyProxy, aBodyID);
				}

				uint16_t maskBits = 0;
				for (int category : tempBoxCollider.getCollideWithCategory())
				{
					maskBits |= (1 << category);
				}

				b2Filter tempFilter = b2Shape_GetFilter(shapeArray[i]);
				tempFilter.maskBits = maskBits;
				tempFilter.categoryBits = (1 << tempBoxCollider.getCollideCategory());

				b2Shape_SetFilter(shapeArray[i], tempFilter);
				boxcounter++;
			}
			catch (std::exception e)
			{
				std::cout << e.what() << std::endl;
			}
		}
		else if (shapeType == b2_circleShape)
		{
			try
			{

				CircleCollider& tempCircleCollider = aBodyProxy.getCircleColliders().at(circlecounter);
				b2Shape_SetDensity(shapeArray[i], aBodyProxy.getDensity());
				b2Shape_SetFriction(shapeArray[i], aBodyProxy.getFriction());
				b2Shape_SetRestitution(shapeArray[i], aBodyProxy.getRestitution());

				if (b2Shape_IsSensor(shapeArray[i]) != tempCircleCollider.getIsTrigger())
				{
					b2DestroyShape(shapeArray[i]);
					createShape(aBodyProxy, aBodyID);
				}

				uint16_t maskBits = 0;
				for (int category : tempCircleCollider.getCollideWithCategory())
				{
					maskBits |= (1 << category);
				}

				b2Filter tempFilter = b2Shape_GetFilter(shapeArray[i]);
				tempFilter.maskBits = maskBits;
				tempFilter.categoryBits = (1 << tempCircleCollider.getCollideCategory());

				b2Shape_SetFilter(shapeArray[i], tempFilter);
				circlecounter++;
			}
			catch (std::exception e)
			{
				std::cout << e.what() << std::endl;
			}
		}
	}
}

void World::updateShapeSize(const BodyProxy& aBodyProxy, const BodyID& aBodyID)
{
	b2BodyId bodyID = convertToB2BodyID(aBodyID);
	int size = aBodyProxy.getBoxColliders().size() + aBodyProxy.getCircleColliders().size();

	b2ShapeId shapeArray[size];

	b2Body_GetShapes(bodyID, shapeArray, size);
	int boxcounter = 0;
	int circlecounter = 0;

	Vector2 scale = aBodyProxy.getScale();

	for (int i = 0; i < size; i++)
	{
		b2ShapeType shapeType = b2Shape_GetType(shapeArray[i]);
		if (shapeType == b2_polygonShape)
		{
			try
			{
				BoxCollider& tempBoxCollider = aBodyProxy.getBoxColliders().at(boxcounter);
				b2Polygon polygon = b2MakeOffsetBox(
					tempBoxCollider.getWidth() * scale.x, tempBoxCollider.getHeight() * scale.y,
					{tempBoxCollider.getTransform().position.x, tempBoxCollider.getTransform().position.y},
					tempBoxCollider.getRotation());
				b2Body_SetAwake(bodyID, true);
				b2Shape_SetPolygon(shapeArray[i], &polygon);
				boxcounter++;
			}
			catch (std::exception e)
			{
				std::cout << e.what() << std::endl;
			}
		}
		else if (shapeType == b2_circleShape)
		{
			try
			{
			}
			catch (std::exception e)
			{
				std::cout << e.what() << std::endl;
			}
		}
	}
}

/**
 * @brief Deletes a body from the box2d world
 * @param aBodyID The body ID to delete
 */
void World::deleteBody(const BodyID& aBodyID)
{
	b2BodyId bodyID = convertToB2BodyID(aBodyID);
	if (b2Body_IsValid(bodyID))
	{
		b2DestroyBody(bodyID);
	}
}

/**
 * @brief Applies a linear force to a body
 * @param aForce The force to apply
 * @param aBodyID The body
 */
void World::applyLinearForce(const std::vector<Vector2>& aForce, const BodyID& aBodyID)
{
	b2BodyId bodyID = convertToB2BodyID(aBodyID);
	for (int i = 0; i < aForce.size(); i++)
	{
		b2Vec2 force = {aForce[i].x, aForce[i].y};
		b2Body_ApplyLinearImpulseToCenter(bodyID, force, true);
	}
}

/**
 * @brief Applies a torque to a body
 * @param aTorque The torque to apply
 * @param aBodyID The body
 */
void World::applyTorque(const std::vector<float>& aTorque, const BodyID& aBodyID)
{
	b2BodyId bodyID = convertToB2BodyID(aBodyID);
	for (int torqueBufferIndex = 0; torqueBufferIndex < aTorque.size(); torqueBufferIndex++)
	{
		b2Body_ApplyTorque(bodyID, aTorque[torqueBufferIndex], true);
	}
}

Vector2 World::getPosition(const BodyID& aBodyID)
{
	b2BodyId bodyID = convertToB2BodyID(aBodyID);
	Vector2 position = {b2Body_GetPosition(bodyID).x, b2Body_GetPosition(bodyID).y};
	return position;
}

float World::getRotation(const BodyID& aBodyID)
{
	b2BodyId bodyID = convertToB2BodyID(aBodyID);

	if (b2Body_IsValid(bodyID))
	{
		float radians = atan2(b2Body_GetRotation(bodyID).s, b2Body_GetRotation(bodyID).c);
		return radians * (180.0f / M_PI);
	}
	return 0;
}

void World::setBodyActivity(bool aState, const BodyID& aBodyID)
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

void World::setGravity(const Vector2& aGravity)
{
	mGravity = aGravity;
	b2World_SetGravity(mWorldID, {aGravity.x, aGravity.y});
}

void World::setLinearVelocity(const Vector2& aVelocity, const BodyID& aBodyID)
{
	b2BodyId bodyID = convertToB2BodyID(aBodyID);
	b2Body_SetLinearVelocity(bodyID, {aVelocity.x, aVelocity.y});
}

void World::setAngularVelocity(float aVelocity, const BodyID& aBodyID)
{
	b2BodyId bodyID = convertToB2BodyID(aBodyID);
	b2Body_SetAngularVelocity(bodyID, aVelocity);
}

Vector2 World::getGravity() { return mGravity; }

void World::setPosition(const Vector2& aPosition, float aRotation, const BodyID& aBodyID)
{
	b2BodyId bodyid = convertToB2BodyID(aBodyID);
	float radians = aRotation * (M_PI / 180.0f);
	b2Rot rot;
	rot.s = sin(radians);
	rot.c = cos(radians);

	b2Body_SetTransform(bodyid, {aPosition.x, aPosition.y}, rot);
}

Vector2 World::getLinearVelocity(const BodyID& aBodyID)
{
	b2BodyId bodyID = convertToB2BodyID(aBodyID);
	b2Vec2 velocity = b2Body_GetLinearVelocity(bodyID);
	return {velocity.x, velocity.y};
}

float World::getAngularVelocity(const BodyID& aBodyID)
{
	b2BodyId bodyID = convertToB2BodyID(aBodyID);
	return b2Body_GetAngularVelocity(bodyID);
}

/**
 * @brief Converts a BodyID to a b2BodyID
 * @param aBodyID The BodyID to convert
 * @return The converted b2BodyID
 */
b2BodyId World::convertToB2BodyID(const BodyID& aBodyID)
{
	b2BodyId bodyID;
	bodyID.index1 = aBodyID.bodyID;
	bodyID.revision = aBodyID.revision;
	bodyID.world0 = aBodyID.world0;
	return bodyID;
}

/**
 * @brief Gets all contact events of the current world step
 * @return The contact events
 */
std::vector<std::pair<int, int>> World::getContactEvents() const
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

/**
 * @brief Gets all sensor events of the current world step
 * @return The sensor events
 */
std::vector<std::pair<int, int>> World::getSensorEvents() const
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
