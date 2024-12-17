#include "Physics/PhysicsEngine.h"
#include "CircleCollider.h"
#include "RigidBody.h"
#include <algorithm>
#include <functional>

PhysicsEngine::PhysicsEngine() : mStep(20.0f / 60.0f), mSubStep(4) {}

PhysicsEngine::~PhysicsEngine() {}

/**
 * @brief Updates the physics engine
 */
void PhysicsEngine::update()
{
	convertToBox2D(mObjects);

	createBodies();

	updateFlags();
	setPositions();
	applyForces();

	mWorld.executeWorldStep(mStep, mSubStep);
	convertFromBox2D(mObjects);

	executeCollisionScripts(mWorld.getContactEvents());
	executeCollisionScripts(mWorld.getSensorEvents());
}

/**
 * @brief Applies forces to the bodies in the world
 */
void PhysicsEngine::applyForces()
{
	for (GameObject& gameObject : mObjects)
	{
		if (gameObject.hasComponent<RigidBody>())
		{
			RigidBody& rigidBody = gameObject.getComponents<RigidBody>()[0];
			mWorld.applyLinearForce(rigidBody.getForcesBuffer(), rigidBody.getBodyId());
			mWorld.applyTorque(rigidBody.getTorqueBuffer(), rigidBody.getBodyId());
			rigidBody.clearForcesBuffer();
			rigidBody.clearTorqueBuffer();
		}
	}
}

/**
 * @brief Sets the positions of the bodies in the world
 */
void PhysicsEngine::setPositions()
{
	for (GameObject& gameObject : mObjects)
	{
		if (gameObject.hasComponent<RigidBody>())
		{
			RigidBody& rigidBody = gameObject.getComponents<RigidBody>()[0];
			Transform transform = gameObject.getTransform();

			Vector2 newPos = Vector2(transform.position.x, transform.position.y);
			if (newPos.x != mWorld.getPosition(rigidBody.getBodyId()).x ||
				newPos.y != mWorld.getPosition(rigidBody.getBodyId()).y)
			{
				mWorld.setPosition(newPos, transform.rotation, rigidBody.getBodyId());
			}
			if (rigidBody.getLinearVelocity() != mWorld.getLinearVelocity(rigidBody.getBodyId()))
			{
				mWorld.setLinearVelocity(rigidBody.getLinearVelocity(), rigidBody.getBodyId());
			}
			if (rigidBody.getAngularVelocity() != mWorld.getAngularVelocity(rigidBody.getBodyId()))
			{
				mWorld.setAngularVelocity(rigidBody.getAngularVelocity(), rigidBody.getBodyId());
			}
		}
	}
}

void PhysicsEngine::setSubStep(int aSubStep) { mSubStep = aSubStep; }

float PhysicsEngine::getSubStep() const { return mSubStep; }

void PhysicsEngine::setStep(float aStep) { mStep = aStep; }

float PhysicsEngine::getStep() const { return mStep; }

/**
 * @brief Executes the collision scripts of the game objects
 * @param aBodyIDs The body IDs of the game objects
 */
void PhysicsEngine::executeCollisionScripts(std::vector<std::pair<int, int>> aBodyIDs)
{

	for (int i = 0; i < aBodyIDs.size(); i++)
	{
		GameObject& gameObjectA = getGameObjectByID(aBodyIDs.at(i).first);
		GameObject& gameObjectB = getGameObjectByID(aBodyIDs.at(i).second);

		if (gameObjectA.hasComponent<IBehaviourScript>())
		{
			std::vector<std::reference_wrapper<IBehaviourScript>> behaviourScript =
				gameObjectA.getComponents<IBehaviourScript>();
			for (int i = 0; i < behaviourScript.size(); i++)
			{
				behaviourScript.at(i).get().onCollide(&gameObjectB);
			}
		}

		if (gameObjectB.hasComponent<IBehaviourScript>())
		{
			if (gameObjectB.hasComponent<IBehaviourScript>())
			{
				std::vector<std::reference_wrapper<IBehaviourScript>> behaviourScript =
					gameObjectB.getComponents<IBehaviourScript>();
				for (int i = 0; i < behaviourScript.size(); i++)
				{
					behaviourScript.at(i).get().onCollide(&gameObjectA);
				}
			}
		}
	}
}

/**
 * @brief Creates the bodies in the world
 */
void PhysicsEngine::createBodies()
{
	for (GameObject& gameObject : mObjects)
	{
		if (gameObject.hasComponent<RigidBody>())
		{
			RigidBody& rigidBody = gameObject.getComponents<RigidBody>()[0];

			if (rigidBody.getBodyId().bodyID == -1)
			{
				BodyProxy bodyProxy = BodyProxy(gameObject);
				BodyID bodyID = mWorld.createBody(bodyProxy);
				rigidBody.setBodyId(bodyID);
			}
		}
	}
}

/**
 * @brief Creates the world
 * @param aGravity The gravity of the world
 */
void PhysicsEngine::createWorld(Vector2 aGravity) { mWorld = World(aGravity); }

void PhysicsEngine::setGravity(Vector2 aGravity) { mWorld.setGravity(aGravity); }

World& PhysicsEngine::getWorld() { return mWorld; }

/**
 * @brief Gets the game objects
 * @param aID The ID of the game object
 * @return The game objects
 */
GameObject& PhysicsEngine::getGameObjectByID(int aID)
{
	for (GameObject& gameObject : mObjects)
	{
		if (gameObject.hasComponent<RigidBody>())
		{
			std::vector<std::reference_wrapper<RigidBody>> rigidBodies = gameObject.getComponents<RigidBody>();
			if (!rigidBodies.empty())
			{
				if (rigidBodies[0].get().getBodyId().bodyID == aID)
				{
					return gameObject;
				}
			}
		}
	}
}

/**
 * @brief Updates the flags of the game objects in the box2d world
 */
void PhysicsEngine::updateFlags()
{
	for (int i = 0; i < mObjects.size(); i++)
	{
		if (mObjects.at(i).get().hasComponent<RigidBody>())
		{
			RigidBody& body = mObjects.at(i).get().getComponents<RigidBody>()[0];
			BodyID bodyID = mObjects.at(i).get().getComponents<RigidBody>()[0].get().getBodyId();
			BodyProxy bodyProxy = BodyProxy(mObjects.at(i));
			mWorld.setBodyActivity(mObjects.at(i).get().getComponents<RigidBody>().at(0).get().isActive(), bodyID);

			if (body.getIsUpdated())
			{
				mWorld.updateBodyProperties(bodyProxy, bodyID);
				body.setIsUpdated(false);
			}

			for (int i = 0; i < bodyProxy.getBoxColliders().size(); i++)
			{
				if (bodyProxy.getBoxColliders().at(i).get().getIsUpdated())
				{
					mWorld.updateShapeProperties(bodyProxy, bodyID);
					bodyProxy.getBoxColliders().at(i).get().setIsUpdated(false);
					break;
				}
			}

			for (int i = 0; i < bodyProxy.getCircleColliders().size(); i++)
			{
				if (bodyProxy.getCircleColliders().at(i).get().getIsUpdated())
				{
					mWorld.updateShapeProperties(bodyProxy, bodyID);
					bodyProxy.getCircleColliders().at(i).get().setIsUpdated(false);
					break;
				}
			}
		}
	}
}

/**
 * @brief Converts Box2D world positions to engine bravo coords
 * @param aGameObjects The game objects
 */
void PhysicsEngine::convertFromBox2D(const std::vector<std::reference_wrapper<GameObject>>& aGameObjects)
{
	for (GameObject& gameObject : aGameObjects)
	{
		if (gameObject.hasComponent<RigidBody>())
		{
			RigidBody& rigidBody = gameObject.getComponents<RigidBody>()[0];
			rigidBody.setLinearVelocity(mWorld.getLinearVelocity(rigidBody.getBodyId()));
			rigidBody.setAngularVelocity(mWorld.getAngularVelocity(rigidBody.getBodyId()));
			if (gameObject.hasComponent<BoxCollider>())
			{
				for (BoxCollider& boxCollider : gameObject.getComponents<BoxCollider>())
				{

					Transform transform = boxCollider.getTransform();
					transform.position.x = transform.position.x - boxCollider.getWidth();
					transform.position.y = transform.position.y - boxCollider.getHeight();

					boxCollider.setWidth(boxCollider.getWidth() * 2);
					boxCollider.setHeight(boxCollider.getHeight() * 2);

					boxCollider.setTransform(transform);
				}
			}

			Vector2 position = mWorld.getPosition(rigidBody.getBodyId());
			float rotation = mWorld.getRotation(rigidBody.getBodyId());

			Transform transform = gameObject.getTransform();
			transform.position = position;
			transform = Transform(Vector2(position.x, position.y));

			transform.rotation = rotation;

			gameObject.setTransform(transform);
		}
	}
}

/**
 * @brief Converts engine bravo coords to Box2D world positions
 * @param aGameObjects The game objects
 */
void PhysicsEngine::convertToBox2D(const std::vector<std::reference_wrapper<GameObject>>& aGameObjects)
{
	for (GameObject& gameObject : aGameObjects)
	{
		if (gameObject.hasComponent<RigidBody>())
		{
			if (gameObject.hasComponent<BoxCollider>())
			{
				for (BoxCollider& boxCollider : gameObject.getComponents<BoxCollider>())
				{
					boxCollider.setWidth(boxCollider.getWidth() / 2);
					boxCollider.setHeight(boxCollider.getHeight() / 2);

					Transform transform = boxCollider.getTransform();
					transform.position.x = transform.position.x + boxCollider.getWidth();
					transform.position.y = transform.position.y + boxCollider.getHeight();
					boxCollider.setTransform(transform);
				}
			}
		}
	}
}

void PhysicsEngine::addObject(GameObject& aObject)
{
	auto it = std::find_if(mObjects.begin(), mObjects.end(),
						   [&aObject](const std::reference_wrapper<GameObject>& wrapper)
						   {
							   return &wrapper.get() == &aObject; // Compare addresses
						   });
	if (it == mObjects.end())
	{
		// Object has not been added yet
		mObjects.push_back(aObject);
	}
}

void PhysicsEngine::removeObject(GameObject& aObject)
{
	for (auto it = mObjects.begin(); it != mObjects.end(); ++it)
	{
		if (&it->get() == &aObject)
		{
			mWorld.deleteBody(it->get().getComponents<RigidBody>()[0].get().getBodyId());
			it->get().getComponents<RigidBody>()[0].get().setBodyId({-1, 0, 0});
			mObjects.erase(it);
			break;
		}
	}
}

const std::vector<std::reference_wrapper<GameObject>>& PhysicsEngine::getObjects() const { return mObjects; }

void PhysicsEngine::clearObjects()
{
	for (GameObject& gameObject : mObjects)
	{
		if (gameObject.hasComponent<RigidBody>())
		{
			mWorld.deleteBody(gameObject.getComponents<RigidBody>()[0].get().getBodyId());

			gameObject.getComponents<RigidBody>()[0].get().setBodyId({-1, 0, 0});
		}
	}
	mObjects.clear();
}
