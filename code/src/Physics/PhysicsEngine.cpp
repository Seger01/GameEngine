#include "Physics/PhysicsEngine.h"
#include "CircleCollider.h"
#include <algorithm>

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
			// mWorld.applyTorque(rigidBody->getTorqueBuffer(), rigidBody->getBodyId());
			rigidBody->clearForcesBuffer();
			// rigidBody->clearTorqueBuffer();

			if (!rigidBody->getTorqueBuffer().empty())
			{
				float xPivot = 0;
				float yPivot = 0;
				int i = 0;

				for (BoxCollider* boxCollider : gameObject.getComponents<BoxCollider>())
				{
					Transform transform = boxCollider->getTransform();

					if (xPivot == 0)
					{
						xPivot += boxCollider->getWidth();
						yPivot += boxCollider->getHeight();
					}
					else
					{
						float extraWidth = xPivot - boxCollider->getWidth() + transform.position.x;
						float extraHeight = yPivot - boxCollider->getHeight() + transform.position.y;

						if (extraWidth < 0)
						{
							xPivot += abs(extraWidth);
						}
						if (extraHeight < 0)
						{
							yPivot += abs(extraHeight);
						}
						i++;
					}
				}
				std::vector<Vector2> impulseLeft;
				std::vector<Vector2> impulseRight;

				for (float force : rigidBody->getTorqueBuffer())
				{
					impulseLeft.push_back(Vector2(0, -force));
					impulseRight.push_back(Vector2(0, force));
				}
				std::cout << "X: " << xPivot << " Y: " << yPivot << std::endl;
				mWorld.applyLinearImpulse(impulseLeft, impulseRight, xPivot, yPivot, rigidBody->getBodyId());
				rigidBody->clearTorqueBuffer();
			}
		}
	}
}

// Sets positions of bodies in world if they differ from the GameObject's position
void PhysicsEngine::setPositions()
{
	for (GameObject& gameObject : mObjects)
	{
		if (gameObject.hasComponent<RigidBody>())
		{
			RigidBody* rigidBody = gameObject.getComponents<RigidBody>()[0];
			Transform transform = gameObject.getTransform();

			Vector2 newPos = Vector2(transform.position.x, transform.position.y);

			// if (newPos != mWorld.getPosition(rigidBody->getBodyId()) ||
			// 	transform.rotation != mWorld.getRotation(rigidBody->getBodyId()))
			// {
			//

			// for (BoxCollider* boxCollider : gameObject.getComponents<BoxCollider>())
			// {
			//
			// 	if (xPivot == 0)
			// 	{
			// 		xPivot += boxCollider->getWidth();
			// 		yPivot += boxCollider->getHeight();
			// 	}
			// 	else
			// 	{
			// 		xPivot += xPivot - boxCollider->getWidth() + transform.position.x;
			// 		yPivot += yPivot - boxCollider->getHeight() + transform.position.y;
			// 	}
			// }
			//
			// float relativeX = transform.position.x + xPivot;
			//
			// float relativeY = transform.position.y + yPivot;
			//
			// float radians = mWorld.getRotation(rigidBody->getBodyId()) * (M_PI / 180.0f);
			//
			// float x = (transform.position.x - xPivot) * cos(radians) - (transform.position.y - yPivot) * sin(radians)
			// + 		  xPivot; float y = (transform.position.x - xPivot) * sin(radians) + (transform.position.y -
			// yPivot) * cos(radians) + 		  yPivot;
			//
			// newPos = Vector2(x, y);
			//

			mWorld.setPosition(newPos, transform.rotation, rigidBody->getBodyId());
			//}
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
			mWorld.setBodyActivity(mObjects.at(i).get().getComponents<RigidBody>().at(0)->isActive(), bodyID);

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

			for (int i = 0; i < bodyProxy.getCircleColliders().size(); i++)
			{
				if (bodyProxy.getCircleColliders().at(i)->getIsUpdated())
				{
					mWorld.updateShapeProperties(bodyProxy, bodyID);
					bodyProxy.getCircleColliders().at(i)->setIsUpdated(false);
					break;
				}
			}
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
				for (BoxCollider* boxCollider : gameObject.getComponents<BoxCollider>())
				{
					boxCollider->setWidth(boxCollider->getWidth() * 2);
					boxCollider->setHeight(boxCollider->getHeight() * 2);

					Transform transform = boxCollider->getTransform();
					transform.position.x = transform.position.x - boxCollider->getWidth() / 2;
					transform.position.y = transform.position.y - boxCollider->getHeight() / 2;
					boxCollider->setTransform(transform);
				}
			}

			Vector2 position = mWorld.getPosition(rigidBody->getBodyId());
			float rotation = mWorld.getRotation(rigidBody->getBodyId());

			Transform transform = gameObject.getTransform();
			transform.position = position;
			transform = Transform(Vector2(position.x, position.y));

			transform.rotation = rotation;

			gameObject.setTransform(transform);
		}
	}
	// for (GameObject& gameObject : aGameObjects)
	// {
	// 	if (gameObject.hasComponent<RigidBody>())
	// 	{
	// 		RigidBody* rigidBody = gameObject.getComponents<RigidBody>()[0];
	// 		float xPivot = 0;
	// 		float yPivot = 0;
	// 		int i = 0;
	//
	// 		if (gameObject.hasComponent<BoxCollider>())
	// 		{
	// 			RigidBody* rigidBody = gameObject.getComponents<RigidBody>()[0];
	// 			for (BoxCollider* boxCollider : gameObject.getComponents<BoxCollider>())
	// 			{
	//
	// 				Transform transform = boxCollider->getTransform();
	//
	// 				if (xPivot == 0)
	// 				{
	// 					xPivot += boxCollider->getWidth();
	// 					yPivot += boxCollider->getHeight();
	// 				}
	// 				else
	// 				{
	// 					float extraWidth = xPivot - boxCollider->getWidth() + transform.position.x;
	// 					float extraHeight = yPivot - boxCollider->getHeight() + transform.position.y;
	//
	// 					if (extraWidth < 0)
	// 					{
	// 						xPivot += abs(extraWidth);
	// 					}
	// 					if (extraHeight < 0)
	// 					{
	// 						yPivot += abs(extraHeight);
	// 					}
	// 					i++;
	// 				}
	//
	// 				boxCollider->setWidth(boxCollider->getWidth() * 2);
	// 				boxCollider->setHeight(boxCollider->getHeight() * 2);
	// 			}
	// 		}
	//
	// 		Vector2 position = mWorld.getPosition(rigidBody->getBodyId());
	// 		position.x = position.x - xPivot;
	// 		position.y = position.y - yPivot;
	// 		float rotation = mWorld.getRotation(rigidBody->getBodyId());
	//
	// 		Transform transform = gameObject.getTransform();
	//
	// 		transform.position = position;
	// 		transform = Transform(Vector2(position.x, position.y));
	//
	// 		transform.rotation = rotation;
	//
	// 		gameObject.setTransform(transform);
	// 	}
	// }
}

// Converts GameObjects to Box2D world positions
void PhysicsEngine::convertToBox2D(const std::vector<std::reference_wrapper<GameObject>>& aGameObjects)
{
	for (GameObject& gameObject : aGameObjects)
	{
		if (gameObject.hasComponent<RigidBody>())
		{
			if (gameObject.hasComponent<BoxCollider>())
			{
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
	// for (GameObject& gameObject : aGameObjects)
	// {
	// 	if (gameObject.hasComponent<RigidBody>())
	// 	{
	// 		if (gameObject.hasComponent<BoxCollider>())
	// 		{
	// 			RigidBody* rigidBody = gameObject.getComponents<RigidBody>()[0];
	//
	// 			float xPivot = 0;
	// 			float yPivot = 0;
	// 			int i = 0;
	//
	// 			for (BoxCollider* boxCollider : gameObject.getComponents<BoxCollider>())
	// 			{
	//
	// 				boxCollider->setWidth(boxCollider->getWidth() / 2);
	// 				boxCollider->setHeight(boxCollider->getHeight() / 2);
	// 				Transform transform = boxCollider->getTransform();
	//
	// 				if (xPivot == 0)
	// 				{
	// 					xPivot += boxCollider->getWidth();
	// 					yPivot += boxCollider->getHeight();
	// 				}
	// 				else
	// 				{
	// 					float extraWidth = xPivot - boxCollider->getWidth() + transform.position.x;
	// 					float extraHeight = yPivot - boxCollider->getHeight() + transform.position.y;
	//
	// 					if (extraWidth < 0)
	// 					{
	// 						xPivot += abs(extraWidth);
	// 					}
	// 					if (extraHeight < 0)
	// 					{
	// 						yPivot += abs(extraHeight);
	// 					}
	// 					i++;
	// 				}
	// 			}
	//
	// 			Transform transform = gameObject.getTransform();
	// 			transform.position.x = transform.position.x + xPivot;
	// 			transform.position.y = transform.position.y + yPivot;
	//
	// 			gameObject.setTransform(transform);
	// 		}
	// 	}
	// }
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
