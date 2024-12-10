#include "Physics/PhysicsEngine.h"

PhysicsEngine::PhysicsEngine() : mStep(20.0f / 60.0f), mSubStep(4) {}

PhysicsEngine::~PhysicsEngine() {}

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

// Applies forces from the forces buffer to the bodies in the world
void PhysicsEngine::applyForces()
{
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
}

// Sets positions of bodies in world if they differ from the GameObject's position
void PhysicsEngine::setPositions()
{
	for (GameObject& gameObject : mObjects)
	{
		if (gameObject.hasComponent<RigidBody>() && gameObject.hasComponent<BoxCollider>())
		{
			RigidBody* rigidBody = gameObject.getComponents<RigidBody>()[0];
			Transform transform = gameObject.getTransform();

			Vector2 newPos = Vector2(transform.position.x, transform.position.y);

			if (newPos != mWorld.getPosition(rigidBody->getBodyId()) ||
				transform.rotation != mWorld.getRotation(rigidBody->getBodyId()))
			{
				mWorld.setPosition(newPos, transform.rotation, rigidBody->getBodyId());
			}
		}
	}
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

// Creates bodies in Box2D world for GameObjects with RigidBody components
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
				rigidBody->setBodyId(bodyID);
			}
		}
	}
}

void PhysicsEngine::createWorld(Vector2 aGravity) { mWorld = World(aGravity); }

World& PhysicsEngine::getWorld() { return mWorld; }

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

// Updates flags for gameObjects rigidbodies and collider shapes
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

// Converts GameObjects from Box2D positions to (local)
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
				transform.position = position;

				transform = Transform(Vector2(position.x, position.y));

				for (BoxCollider* boxCollider : gameObject.getComponents<BoxCollider>())
				{
					boxCollider->setWidth(boxCollider->getWidth() * 2);
					boxCollider->setHeight(boxCollider->getHeight() * 2);

					Transform transform = boxCollider->getTransform();
					transform.position.x = transform.position.x - boxCollider->getWidth() / 2;
					transform.position.y = transform.position.y - boxCollider->getHeight() / 2;
					boxCollider->setTransform(transform);
				}

				transform.rotation = rotation;

				gameObject.setTransform(transform);
			}
		}
	}
}

// Converts GameObjects to Box2D world positions
void PhysicsEngine::convertToBox2D(const std::vector<std::reference_wrapper<GameObject>>& aGameObjects)
{

	for (GameObject& gameObject : aGameObjects)
	{
		if (gameObject.hasComponent<RigidBody>() && gameObject.hasComponent<BoxCollider>())
		{
			Transform transform = gameObject.getTransform();
			BoxCollider* boxCollider = gameObject.getComponents<BoxCollider>()[0];

			for (BoxCollider* boxCollider : gameObject.getComponents<BoxCollider>())
			{
				boxCollider->setWidth(boxCollider->getWidth() / 2);
				boxCollider->setHeight(boxCollider->getHeight() / 2);

				Transform transform = boxCollider->getTransform();
				transform.position.x = transform.position.x + boxCollider->getWidth();
				transform.position.y = transform.position.y + boxCollider->getHeight();
				boxCollider->setTransform(transform);
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
			mWorld.deleteBody(it->get().getComponents<RigidBody>()[0]->getBodyId());
			it->get().getComponents<RigidBody>()[0]->setBodyId({-1, 0, 0});
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
			mWorld.deleteBody(gameObject.getComponents<RigidBody>()[0]->getBodyId());

			gameObject.getComponents<RigidBody>()[0]->setBodyId({-1, 0, 0});
		}
	}
	mObjects.clear();
}
