#include "UpdateQueue.h"

#include "Animation.h"
#include "EngineBravo.h"
#include "GameObject.h"
#include "NetworkObject.h"
#include "ParticleEmitter.h"
#include "Text.h"

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
		EngineBravo::getInstance().getRenderSystem().removeObject(gameObject);
		if (typeid(gameObject) == typeid(Text))
		{
			EngineBravo::getInstance().getRenderSystem().addObject(gameObject);
		}
		if (gameObject.hasComponent<Animation>())
		{
			EngineBravo::getInstance().getRenderSystem().addObject(gameObject);
		}
		if (gameObject.hasComponent<Sprite>())
		{
			EngineBravo::getInstance().getRenderSystem().addObject(gameObject);
		}
		if (gameObject.hasComponent<ParticleEmitter>())
		{
			EngineBravo::getInstance().getRenderSystem().addObject(gameObject);
		}
		// Resource manager: does not use a list of game objects
		// Particle system
		EngineBravo::getInstance().getParticleSystem().removeObject(gameObject);
		if (gameObject.hasComponent<ParticleEmitter>())
		{
			EngineBravo::getInstance().getParticleSystem().addObject(gameObject);
		}
		// Network manager:
		EngineBravo::getInstance().getNetworkManager().removeObject(gameObject);
		if (gameObject.hasComponent<NetworkObject>())
		{
			EngineBravo::getInstance().getNetworkManager().addObject(gameObject);
		}
		// Event manager: does not use a list of game objects
		// Save game manager: does not use a list of game objects
		// Audio manager
		EngineBravo::getInstance().getAudioManager().removeObject(gameObject);
		if (gameObject.hasComponent<AudioSource>())
		{
			EngineBravo::getInstance().getAudioManager().addObject(gameObject);
		}
		// UI manager:
		EngineBravo::getInstance().getUIManager().removeObject(gameObject);
		if (typeid(gameObject) == typeid(Button))
		{
			EngineBravo::getInstance().getUIManager().addObject(gameObject);
		}
		// Physics manager
		EngineBravo::getInstance().getPhysicsManager().getPhysicsEngine().removeObject(gameObject);
		if (gameObject.hasComponent<RigidBody>())
		{
			EngineBravo::getInstance().getPhysicsManager().getPhysicsEngine().addObject(gameObject);
		}
		if (gameObject.hasComponent<BoxCollider>())
		{
			EngineBravo::getInstance().getPhysicsManager().getPhysicsEngine().addObject(gameObject);
		}
	}
	mUpdateObjects.clear();
}

void UpdateQueue::updateRemovals()
{
	for (GameObject* gameObject :
		 EngineBravo::getInstance().getSceneManager().getCurrentScene()->getGameObjectsToBeRemoved())
	{
		// Scene manager: does not use a list of game objects
		// Render system
		EngineBravo::getInstance().getRenderSystem().removeObject(*gameObject);
		// Resource manager: does not use a list of game objects
		// Particle system
		EngineBravo::getInstance().getParticleSystem().removeObject(*gameObject);
		// Network manager:
		EngineBravo::getInstance().getNetworkManager().removeObject(*gameObject);
		// Event manager: does not use a list of game objects
		// Save game manager: does not use a list of game objects
		// Audio manager
		EngineBravo::getInstance().getAudioManager().removeObject(*gameObject);
		// UI manager
		EngineBravo::getInstance().getUIManager().removeObject(*gameObject);
		// Physics manager
		EngineBravo::getInstance().getPhysicsManager().getPhysicsEngine().removeObject(*gameObject);
	}
}

void UpdateQueue::clearManagerObjects()
{
	EngineBravo::getInstance().getRenderSystem().clearObjects();
	EngineBravo::getInstance().getParticleSystem().clearObjects();
	EngineBravo::getInstance().getNetworkManager().clearObjects();
	EngineBravo::getInstance().getAudioManager().clearObjects();
	EngineBravo::getInstance().getUIManager().clearObjects();
	EngineBravo::getInstance().getPhysicsManager().getPhysicsEngine().clearObjects();
}