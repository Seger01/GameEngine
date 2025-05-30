#include "Physics/PhysicsEngine.h"

/**
 * @file PhysicsEngine.cpp
 *
 * @brief This file contains the implementation of the PhysicsEngine class which is used to
 * store the properties of a physics engine and send it to the box2d world
 *
 */

/**
 * @brief Constructs a PhysicsEngine instance with default values
 */
PhysicsEngine::PhysicsEngine() : mStep(0.02f), mSubStep(4) {}

/**
 * @brief Creates the world
 * @param aGravity The gravity of the world
 */
void PhysicsEngine::createWorld(const Vector2& aGravity) { mWorld = World(aGravity); }

/**
 * @brief Creates the bodies in the box2d world
 */
void PhysicsEngine::createBodies()
{
	for (GameObject& gameObject : mObjects)
	{
		if (gameObject.hasComponent<RigidBody>())
		{
			RigidBody& rigidBody = gameObject.getComponents<RigidBody>()[0];

			// Checks if the body ha sbeen created (-1 equals not created)
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

			// Sets values if changed
			if (newPos.x != mWorld.getPosition(rigidBody.getBodyId()).x ||
				newPos.y != mWorld.getPosition(rigidBody.getBodyId()).y ||
				transform.rotation != mWorld.getRotation(rigidBody.getBodyId()))
			{
				mWorld.setPosition(newPos, transform.rotation, rigidBody.getBodyId());
			}
			mWorld.updateShapeSize(BodyProxy(gameObject), rigidBody.getBodyId());

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
 * @brief Executes the collision scripts of the game objects if collision occurred in box2d world
 * @param aBodIDs vector containing body IDs of collided objects within the current step
 */
void PhysicsEngine::executeCollisionScripts(const std::vector<std::pair<int, int>>& aBodyIDs)
{

	for (int i = 0; i < aBodyIDs.size(); i++)
	{
		// Get game objects by box2d ID to use for collision
		GameObject& gameObjectA = getGameObjectByID(aBodyIDs.at(i).first);
		GameObject& gameObjectB = getGameObjectByID(aBodyIDs.at(i).second);

		if (gameObjectA.hasComponent<IBehaviourScript>())
		{
			// Get behaviour script of collided object
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
				// Get behaviour script of collided object

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
				// Sets colliders to half size and offsets them to match Box2D positioning
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
				// Sets colliders to full size and offsets them to match engine bravo positioning
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
			transform.position = Vector2(position.x, position.y);

			transform.rotation = rotation;

			gameObject.setTransform(transform);
		}
	}
}

/**
 * @brief Gets the game objects
 * @param aID The ID of the game object
 * @return The game objects
 */
GameObject& PhysicsEngine::getGameObjectByID(int aBodyID)
{
	for (GameObject& gameObject : mObjects)
	{
		if (gameObject.hasComponent<RigidBody>())
		{
			std::vector<std::reference_wrapper<RigidBody>> rigidBodies = gameObject.getComponents<RigidBody>();
			if (!rigidBodies.empty())
			{
				if (rigidBodies[0].get().getBodyId().bodyID == aBodyID)
				{
					return gameObject;
				}
			}
		}
	}
	return mObjects[0];
}

float PhysicsEngine::getSubStep() const { return mSubStep; }

float PhysicsEngine::getStep() const { return mStep; }

World& PhysicsEngine::getWorld() { return mWorld; }

void PhysicsEngine::setStep(float aStep) { mStep = aStep; }

void PhysicsEngine::setSubStep(int aSubStep) { mSubStep = aSubStep; }

void PhysicsEngine::setGravity(const Vector2& aGravity) { mWorld.setGravity(aGravity); }

Vector2 PhysicsEngine::getGravity() const { return mWorld.getGravity(); }

/**
 * @brief Adds a game object to the physics engine
 * @param aObject The game object
 */
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

/**
 * @brief Removes a game object from the physics engine
 * @param aObject The game object
 */
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

/**
 * @brief Clears the objects from the physics engine
 */
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
