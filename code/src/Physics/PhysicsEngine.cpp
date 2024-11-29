#include "Physics/PhysicsEngine.h"
#include "BoxCollider.h"
#include "GameObject.h"
#include "RigidBody.h"
#include "box2d/box2d.h"
#include <functional>

PhysicsEngine::PhysicsEngine() {}

PhysicsEngine::~PhysicsEngine() {}

void PhysicsEngine::update()
{
	convertToBox2D(mObjects);

	createBodies();
	updateFlags();
	for (GameObject& gameObject : mObjects)
	{
		if (gameObject.hasComponent<RigidBody>() && gameObject.hasComponent<BoxCollider>())
		{

			RigidBody* rigidBody = gameObject.getComponents<RigidBody>()[0];

			Transform transform = gameObject.getTransform();
			transform.position.x = transform.position.x - gameObject.getComponents<BoxCollider>()[0]->getWidth();
			transform.position.y = transform.position.y - gameObject.getComponents<BoxCollider>()[0]->getHeight();

			Vector2 newPos = Vector2(transform.position.x, transform.position.y);

			if (newPos != mWorld.getPosition(rigidBody->getBodyId()))
			{

				mWorld.setPosition(newPos, rigidBody->getBodyId());
			}
		}
		// else if (gameObject.hasComponent<RigidBody>() && gameObject.hasComponent<CircleCollider>())
		// {
		// 	RigidBody* rigidBody = gameObject.getComponents<RigidBody>()[0];
		//
		// 	Transform transform = gameObject.getTransform();
		// 	transform.position.x = transform.position.x - gameObject.getComponents<CircleCollider>()[0]->getRadius();
		// 	transform.position.y = transform.position.y - gameObject.getComponents<CircleCollider>()[0]->getRadius();
		//
		// 	Vector2 newPos = Vector2(transform.position.x, transform.position.y);
		//
		// 	if (newPos != mWorld.getPosition(rigidBody->getBodyId()))
		// 	{
		//
		// 		mWorld.setPosition(newPos, rigidBody->getBodyId());
		// 	}
		// }
	}

	for (GameObject& gameObject : mObjects)
	{
		if (gameObject.hasComponent<RigidBody>())
		{

			RigidBody* rigidBody = gameObject.getComponents<RigidBody>()[0];

			mWorld.applyLinearForce(rigidBody->getForcesBuffer(), rigidBody->getBodyId());
			mWorld.applyTorque(rigidBody->getTorqueBuffer(), rigidBody->getBodyId());
			rigidBody->clearForcesBuffer();
			rigidBody->clearTorqueBuffer();
		}
	}

	float timeStep = 20.0f / 60.0f;
	int velocityIterations = 4;
	int positionIterations = 2;
	mWorld.executeWorldStep(timeStep, velocityIterations);

	executeCollisionScripts(mWorld.getContactEvents());
	executeCollisionScripts(mWorld.getSensorEvents());
	convertFromBox2D(mObjects);
}

void PhysicsEngine::setSubStep(int aSubStep) { mSubStep = aSubStep; }

float PhysicsEngine::getSubStep() const { return mSubStep; }

void PhysicsEngine::setStep(float aStep) { mStep = aStep; }

float PhysicsEngine::getStep() const { return mStep; }

void PhysicsEngine::executeCollisionScripts(std::vector<std::pair<int, int>> aBodyIDs)
{

	for (int i = 0; i < aBodyIDs.size(); i++)
	{
		GameObject* gameObjectA = getGameObjectByID(aBodyIDs.at(i).first);
		GameObject* gameObjectB = getGameObjectByID(aBodyIDs.at(i).second);

		if (gameObjectA != nullptr)
		{

			if (gameObjectA->hasComponent<IBehaviourScript>())
			{
				std::vector<IBehaviourScript*> behaviourScript = gameObjectA->getComponents<IBehaviourScript>();
				for (int i = 0; i < behaviourScript.size(); i++)
				{
					behaviourScript.at(i)->onCollide(gameObjectB);
				}
			}
		}

		if (gameObjectB != nullptr)
		{

			if (gameObjectB->hasComponent<IBehaviourScript>())
			{
				if (gameObjectB->hasComponent<IBehaviourScript>())
				{
					std::vector<IBehaviourScript*> behaviourScript = gameObjectB->getComponents<IBehaviourScript>();
					for (int i = 0; i < behaviourScript.size(); i++)
					{
						behaviourScript.at(i)->onCollide(gameObjectA);
					}
				}
			}
		}
	}
}

void PhysicsEngine::createBodies()
{
	for (GameObject& gameObject : mObjects)
	{

		if (gameObject.hasComponent<RigidBody>())
		{
			RigidBody* rigidBody = gameObject.getComponents<RigidBody>()[0];

			if (rigidBody->getBodyId().bodyID == -1)
			{

				BodyProxy bodyProxy = BodyProxy(gameObject);

				BodyID bodyID = mWorld.createBody(bodyProxy);
				// std::cout << "create body: " << bodyID.world0 << bodyID.bodyID << bodyID.revision << std::endl;
				rigidBody->setBodyId(bodyID);
			}
		}
	}
}

void PhysicsEngine::deleteBodies() {}

void PhysicsEngine::createWorld(Vector2 aGravity) { mWorld = World(aGravity); }

World& PhysicsEngine::getWorld() { return mWorld; }

void PhysicsEngine::reset()
{
	mWorld.resetWorld();
	for (GameObject& gameObject : mObjects)
	{
		std::vector<RigidBody*> rigidBodies = gameObject.getComponents<RigidBody>();
		for (int rigidBodyIndex = 0; rigidBodyIndex < rigidBodies.size(); rigidBodyIndex++)
		{
			rigidBodies.at(rigidBodyIndex)->setBodyId({-1, 0, 0});
		}
	}
}

// Checks and returns GameObject if BodyID exists within world
GameObject* PhysicsEngine::getGameObjectByID(int aID)
{
	for (GameObject& gameObject : mObjects)
	{
		if (gameObject.hasComponent<RigidBody>())
		{
			std::vector<RigidBody*> rigidBodies = gameObject.getComponents<RigidBody>();
			if (!rigidBodies.empty())
			{
				if (rigidBodies[0]->getBodyId().bodyID == aID)
				{
					return &gameObject;
				}
			}
		}
	}
	return nullptr;
}

void PhysicsEngine::updateFlags()
{
	for (int i = 0; i < mObjects.size(); i++)
	{
		if (mObjects.at(i).get().hasComponent<RigidBody>())
		{
			RigidBody* body = mObjects.at(i).get().getComponents<RigidBody>()[0];
			BodyID bodyID = mObjects.at(i).get().getComponents<RigidBody>()[0]->getBodyId();
			BodyProxy bodyProxy = BodyProxy(mObjects.at(i));

			if (body->getIsUpdated())
			{
				mWorld.updateBodyProperties(bodyProxy, bodyID);

				body->setIsUpdated(false);
			}
			for (int i = 0; i < bodyProxy.getBoxColliders().size(); i++)
			{
				if (bodyProxy.getBoxColliders().at(i)->getIsUpdated())
				{
					mWorld.updateShapeProperties(bodyProxy, bodyID);
					bodyProxy.getBoxColliders().at(i)->setIsUpdated(false);
					break;
				}
			}

			mWorld.setBodyActivity(mObjects.at(i).get().getComponents<RigidBody>().at(0)->isActive(), bodyID);
		}
	}
}

void PhysicsEngine::convertFromBox2D(const std::vector<std::reference_wrapper<GameObject>>& aGameObjects)
{
	for (GameObject& gameObject : aGameObjects)
	{
		if (gameObject.hasComponent<RigidBody>())
		{
			RigidBody* rigidBody = gameObject.getComponents<RigidBody>()[0];

			if (gameObject.hasComponent<BoxCollider>())
			{
				Vector2 position = mWorld.getPosition(rigidBody->getBodyId());
				Transform transform = gameObject.getTransform();
				float rotation = mWorld.getRotation(rigidBody->getBodyId());
				BoxCollider* boxCollider = gameObject.getComponents<BoxCollider>()[0];
				transform.position = position;

				transform = Transform(Vector2(position.x, position.y));

				transform.position.x = (transform.position.x - boxCollider->getWidth());
				transform.position.y = (transform.position.y - boxCollider->getHeight());
				transform.rotation = rotation;

				for (BoxCollider* boxCollider : gameObject.getComponents<BoxCollider>())
				{
					boxCollider->setWidth(boxCollider->getWidth() * 2);
					boxCollider->setHeight(boxCollider->getHeight() * 2);
				}

				// for (CircleCollider* circleCollider : gameObject.getComponents<CircleCollider>())
				// {
				// 	circleCollider->setRadius(circleCollider->getRadius() * 2);
				// }

				gameObject.setTransform(transform);
			}
			// else if (gameObject.hasComponent<CircleCollider>())
			// {
			// 	Vector2 position = mWorld.getPosition(rigidBody->getBodyId());
			// 	Transform transform = gameObject.getTransform();
			// 	float rotation = mWorld.getRotation(rigidBody->getBodyId());
			// 	CircleCollider* circleCollider = gameObject.getComponents<CircleCollider>()[0];
			// 	transform.position = position;
			//
			// 	transform = Transform(Vector2(position.x, position.y));
			//
			// 	transform.position.x = (transform.position.x - circleCollider->getRadius());
			// 	transform.position.y = (transform.position.y - circleCollider->getRadius());
			// 	transform.rotation = rotation;
			//
			// 	for (BoxCollider* boxCollider : gameObject.getComponents<BoxCollider>())
			// 	{
			// 		boxCollider->setWidth(boxCollider->getWidth() * 2);
			// 		boxCollider->setHeight(boxCollider->getHeight() * 2);
			// 	}
			//
			// 	for (CircleCollider* circleCollider : gameObject.getComponents<CircleCollider>())
			// 	{
			// 		circleCollider->setRadius(circleCollider->getRadius() * 2);
			// 	}
			//
			// 	gameObject.setTransform(transform);
			// }
		}
	}
}

void PhysicsEngine::convertToBox2D(const std::vector<std::reference_wrapper<GameObject>>& aGameObjects)
{

	for (GameObject& gameObject : aGameObjects)
	{
		if (gameObject.hasComponent<RigidBody>() && gameObject.hasComponent<BoxCollider>())
		{
			Transform transform = gameObject.getTransform();
			BoxCollider* boxCollider = gameObject.getComponents<BoxCollider>()[0];
			transform.position.x = (transform.position.x + boxCollider->getWidth());
			transform.position.y = (transform.position.y + boxCollider->getHeight());
			transform.rotation = transform.rotation;

			for (BoxCollider* boxCollider : gameObject.getComponents<BoxCollider>())
			{
				boxCollider->setWidth(boxCollider->getWidth() / 2);
				boxCollider->setHeight(boxCollider->getHeight() / 2);
			}

			// for (CircleCollider* circleCollider : gameObject.getComponents<CircleCollider>())
			// {
			// 	circleCollider->setRadius(circleCollider->getRadius() / 2);
			// }
			//
			gameObject.setTransform(transform);
		}
		// else if (gameObject.hasComponent<RigidBody>() && gameObject.hasComponent<CircleCollider>())
		// {
		// 	Transform transform = gameObject.getTransform();
		// 	CircleCollider* circleCollider = gameObject.getComponents<CircleCollider>()[0];
		// 	transform.position.x = (transform.position.x + circleCollider->getRadius());
		// 	transform.position.y = (transform.position.y + circleCollider->getRadius());
		// 	transform.rotation = transform.rotation;
		//
		// 	for (BoxCollider* boxCollider : gameObject.getComponents<BoxCollider>())
		// 	{
		// 		boxCollider->setWidth(boxCollider->getWidth() / 2);
		// 		boxCollider->setHeight(boxCollider->getHeight() / 2);
		// 	}
		//
		// 	for (CircleCollider* circleCollider : gameObject.getComponents<CircleCollider>())
		// 	{
		// 		circleCollider->setRadius(circleCollider->getRadius() / 2);
		// 	}
		//
		// 	gameObject.setTransform(transform);
		// }
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

	//   createBodies();
}

void PhysicsEngine::removeObject(GameObject& aObject)
{
	for (auto it = mObjects.begin(); it != mObjects.end(); ++it)
	{
		if (&it->get() == &aObject)
		{
			BodyID bodyID = it->get().getComponents<RigidBody>()[0]->getBodyId();
			mWorld.deleteBody(it->get().getComponents<RigidBody>().at(0)->getBodyId());
			mObjects.erase(it);
			break;
		}
	}
}

const std::vector<std::reference_wrapper<GameObject>>& PhysicsEngine::getObjects() const { return mObjects; }

void PhysicsEngine::clearObjects() { mObjects.clear(); }
