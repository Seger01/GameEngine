/**
 * @file ParticleSystem.cpp
 * @brief Implementation of the ParticleSystem class.
 */

#include "ParticleSystem.h"
#include "GameObject.h"
#include "ParticleEmitter.h"

/**
 * @brief Constructor for the ParticleSystem class.
 */
ParticleSystem::ParticleSystem() {}

/**
 * @brief Updates all particle emitters in the game.
 */
void ParticleSystem::update()
{
	for (GameObject& gameObject : mObjects)
	{
		if (gameObject.isActive())
		{
			for (auto particleEmitter : gameObject.getComponents<ParticleEmitter>())
			{
				particleEmitter->update();
			}
		}
	}
}

/**
 * @brief Adds a GameObject to the particle system.
 * @param aObject The GameObject to add.
 */
void ParticleSystem::addObject(GameObject& aObject)
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
 * @brief Removes a GameObject from the particle system.
 * @param aObject The GameObject to remove.
 */
void ParticleSystem::removeObject(GameObject& aObject)
{
	auto it = std::remove_if(mObjects.begin(), mObjects.end(), [&aObject](const std::reference_wrapper<GameObject>& obj)
							 { return &obj.get() == &aObject; });
	if (it != mObjects.end())
	{
		mObjects.erase(it, mObjects.end());
	}
}

/**
 * @brief Retrieves all GameObjects in the particle system.
 * @return A vector of GameObject references.
 */
const std::vector<std::reference_wrapper<GameObject>>& ParticleSystem::getObjects() const { return mObjects; }

/**
 * @brief Clears all GameObjects from the particle system.
 */
void ParticleSystem::clearObjects() { mObjects.clear(); }
