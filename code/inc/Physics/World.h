#pragma once
/**
 * @file World.h
 *
 * @brief Contains the Box2D physics library and its functions used to create the physics engine for engine bravo
 *
 */

#include "Physics/BodyProxy.h"
#include "Vector2.h"
#include "box2d/box2d.h"
#include "box2d/id.h"
#include <vector>

class World
{
public:
	World(const Vector2& aGravity);
	World();

	void executeWorldStep(float step, int subStep);

	BodyID createBody(const BodyProxy& aBodyProxy);
	void createShape(const BodyProxy& aBodyProxy, const BodyID& aBodyID);

	void updateBodyPosition(const BodyProxy& aBodyProxy, const BodyID& aBodyID);
	void updateBodyProperties(const BodyProxy& aBodyProxy, const BodyID& aBodyID);
	void updateShapeProperties(const BodyProxy& aBodyProxy, const BodyID& aBodyID);
	void updateShapeSize(const BodyProxy& aBodyProxy, const BodyID& aBodyID);

	void deleteBody(const BodyID& aBodyID);

	void applyLinearForce(const std::vector<Vector2>& aForce, const BodyID& aBodyID);
	void applyTorque(const std::vector<float>& aTorque, const BodyID& aBodyID);

	void setGravity(const Vector2& aGravity);
	void setPosition(const Vector2& aPosition, float aRotation, const BodyID& aBodyID);
	void setRotation(float aRotation, const BodyID& aBodyID);
	void setLinearVelocity(const Vector2& aVelocity, const BodyID& aBodyID);
	void setAngularVelocity(float aVelocity, const BodyID& aBodyID);
	void setBodyActivity(bool aState, const BodyID& aBodyID);

	Vector2 getGravity() const;
	Vector2 getPosition(const BodyID& aBodyID);
	float getRotation(const BodyID& aBodyID);
	Vector2 getLinearVelocity(const BodyID& aBodyID);
	float getAngularVelocity(const BodyID& aBodyID);

	b2BodyId convertToB2BodyID(const BodyID& aBodyID);
	std::vector<std::pair<int, int>> getContactEvents() const;
	std::vector<std::pair<int, int>> getSensorEvents() const;

private:
	friend class PhysicsEngine;

private:
	/// @brief The box2d world id
	b2WorldId mWorldID;
	/// @brief The gravity of the world
	Vector2 mGravity;
};
