#include "UpdateQueue.h"

#include "Animation.h"
#include "EngineBravo.h"
#include "GameObject.h"
#include "NetworkObject.h"
#include "ParticleEmitter.h"
#include "Text.h"

UpdateQueue::UpdateQueue() : mEngine(EngineBravo::getInstance()) {}

void UpdateQueue::addToUpdateObjects(GameObject& aGameObject)
{
	auto it = std::find_if(mUpdateObjects.begin(), mUpdateObjects.end(),
						   [&aGameObject](const std::reference_wrapper<GameObject>& wrapper)
						   {
							   return &wrapper.get() == &aGameObject; // Compare addresses
						   });
	if (it == mUpdateObjects.end())
	{
		if (aGameObject.hasComponent<RigidBody>())
		{
			BodyID bodyIDA = aGameObject.getComponents<RigidBody>()[0]->getBodyId();
			std::cout << "UpdateQueue::addToUpdateObjects(): adding " << bodyIDA.world0 << bodyIDA.bodyID
					  << bodyIDA.revision << std::endl;
		}
		mUpdateObjects.push_back(aGameObject);
	}
}

void UpdateQueue::clearUpdateObjects() { mUpdateObjects.clear(); }

void UpdateQueue::updateAdditions()
{
	// Iterate through the objects and update each manager
	for (GameObject& gameObject : mUpdateObjects)
	{
		// Scene manager: does not use a list of game objects
		// Render system
		mEngine.getRenderSystem().removeObject(gameObject);
		if (typeid(gameObject) == typeid(Text))
		{
			mEngine.getRenderSystem().addObject(gameObject);
		}
		if (gameObject.hasComponent<Animation>())
		{
			mEngine.getRenderSystem().addObject(gameObject);
		}
		if (gameObject.hasComponent<Sprite>())
		{
			mEngine.getRenderSystem().addObject(gameObject);
		}
		if (gameObject.hasComponent<ParticleEmitter>())
		{
			mEngine.getRenderSystem().addObject(gameObject);
		}
		// Resource manager: does not use a list of game objects
		// Particle system
		mEngine.getParticleSystem().removeObject(gameObject);
		if (gameObject.hasComponent<ParticleEmitter>())
		{
			mEngine.getParticleSystem().addObject(gameObject);
		}
		// Network manager:
		mEngine.getNetworkManager().removeObject(gameObject);
		if (gameObject.hasComponent<NetworkObject>())
		{
			mEngine.getNetworkManager().addObject(gameObject);
		}
		// Event manager: does not use a list of game objects
		// Save game manager: does not use a list of game objects
		// Audio manager
		mEngine.getAudioManager().removeObject(gameObject);
		if (gameObject.hasComponent<AudioSource>())
		{
			mEngine.getAudioManager().addObject(gameObject);
		}
		// UI manager:
		mEngine.getUIManager().removeObject(gameObject);
		if (typeid(gameObject) == typeid(Button))
		{
			mEngine.getUIManager().addObject(gameObject);
		}
		// Physics manager
		mEngine.getPhysicsManager().getPhysicsEngine().removeObject(gameObject);
		if (gameObject.hasComponent<RigidBody>())
		{
			mEngine.getPhysicsManager().getPhysicsEngine().addObject(gameObject);
		}
		if (gameObject.hasComponent<BoxCollider>())
		{
			mEngine.getPhysicsManager().getPhysicsEngine().addObject(gameObject);
		}
	}
	mUpdateObjects.clear();
}

void UpdateQueue::updateRemovals()
{
	for (GameObject* gameObject : mEngine.getSceneManager().getCurrentScene()->getGameObjectsToBeRemoved())
	{
		// Scene manager: does not use a list of game objects
		// Render system
		mEngine.getRenderSystem().removeObject(*gameObject);
		// Resource manager: does not use a list of game objects
		// Particle system
		mEngine.getParticleSystem().removeObject(*gameObject);
		// Network manager:
		mEngine.getNetworkManager().removeObject(*gameObject);
		// Event manager: does not use a list of game objects
		// Save game manager: does not use a list of game objects
		// Audio manager
		mEngine.getAudioManager().removeObject(*gameObject);
		// UI manager
		mEngine.getUIManager().removeObject(*gameObject);
		// Physics manager
		mEngine.getPhysicsManager().getPhysicsEngine().removeObject(*gameObject);
	}
}

void UpdateQueue::clearManagerObjects()
{
	mEngine.getRenderSystem().clearObjects();
	mEngine.getParticleSystem().clearObjects();
	mEngine.getNetworkManager().clearObjects();
	mEngine.getAudioManager().clearObjects();
	mEngine.getUIManager().clearObjects();
	mEngine.getPhysicsManager().getPhysicsEngine().clearObjects();
}