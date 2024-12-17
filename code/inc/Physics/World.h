#ifndef WORLD_H
#define WORLD_H

#include "Physics/BodyProxy.h"
#include "Vector2.h"
#include "box2d/box2d.h"
#include "box2d/id.h"
#include <vector>

class World
{
public:
	World(Vector2 aGravity);
	World();

	void executeWorldStep(float step, int subStep);

	BodyID createBody(BodyProxy& aBodyProxy);
	void createShape(BodyProxy& aBodyProxy, BodyID aBodyID);

	void updateBodyPosition(BodyProxy& aBodyProxy, BodyID aBodyID);
	void updateBodyProperties(BodyProxy& aBodyProxy, BodyID aBodyID);
	void updateShapeProperties(BodyProxy& aBodyProxy, BodyID aBodyID);

	void deleteBody(BodyID aBodyID);

	void applyLinearForce(std::vector<Vector2> aForce, BodyID aBodyID);
	void applyTorque(std::vector<float> aTorque, BodyID aBodyID);

	void setGravity(Vector2 aGravity);
	void setPosition(Vector2 aPosition, float aRotation, BodyID aBodyID);
	void setRotation(float aRotation, BodyID aBodyID);
	void setLinearVelocity(Vector2 aVelocity, BodyID aBodyID);
	void setAngularVelocity(float aVelocity, BodyID aBodyID);
	void setBodyActivity(bool aState, BodyID aBodyID);

	Vector2 getGravity();
	Vector2 getPosition(BodyID aBodyID);
	float getRotation(BodyID aBodyID);
	Vector2 getLinearVelocity(BodyID aBodyID);
	float getAngularVelocity(BodyID aBodyID);

	b2BodyId convertToB2BodyID(BodyID aBodyID);
	std::vector<std::pair<int, int>> getContactEvents();
	std::vector<std::pair<int, int>> getSensorEvents();

private:
	b2WorldId mWorldID;
	Vector2 mGravity;
};

#endif // WORLD_H
