#include "Scene.h"

#include "Engine/EngineBravo.h"

#include <algorithm>
#include <functional>
#include <iostream>
#include <stdexcept>

Scene::Scene(std::string aSceneName, int aSceneID)
	: mSceneName(std::move(aSceneName)), mSceneID(aSceneID), mActiveCameraIndex(0)
{
}

Scene::~Scene() = default;

void Scene::update()
{
	if (mGameObjectsToRemove.size() > 0)
	{
		// std::cout << "Removing GameObject in the update function" << std::endl;
		for (auto& toBeRemoved : mGameObjectsToRemove)
		{
			removeGameObject(&toBeRemoved.get());
		}
		mGameObjectsToRemove.clear();
	}
}

/**
 * @brief Removes a GameObject from the scene
 * @note This functions also removes it from the persistent game objects list
 *
 * @param aObject The GameObject to remove
 */
void Scene::removeGameObject(GameObject* aObject)
{
	for (int i = 0; i < mGameObjects.size(); i++)
	{
		if (mGameObjects[i].get() == aObject)
		{
			mGameObjects.erase(mGameObjects.begin() + i);
			break;
		}
	}
	removePersistentGameObject(aObject);
	// std::cerr << "RemoveGameObject called but no matching object found" << std::endl;
}

std::vector<std::reference_wrapper<GameObject>> Scene::getGameObjects() const
{
	std::vector<std::reference_wrapper<GameObject>> gameObjectRefs;
	for (const auto& gameObject : mGameObjects)
	{
		gameObjectRefs.push_back(*gameObject.get());
	}
	return gameObjectRefs;
}

std::vector<std::reference_wrapper<GameObject>> Scene::getGameObjectsWithTag(const std::string& tag)
{
	std::vector<std::reference_wrapper<GameObject>> objectsWithTag;
	for (const auto& obj : mGameObjects)
	{
		if (obj->getTag() == tag)
		{
			objectsWithTag.push_back(*obj.get());
		}
	}
	return objectsWithTag;
}

void Scene::addGameObject(GameObject* object)
{
	if (object)
	{
		mGameObjects.push_back(std::unique_ptr<GameObject>(object));
		EngineBravo::getInstance().getUpdateQueue().addToUpdateObjects(*object);
	}
}

void Scene::requestGameObjectRemoval(int id)
{
	for (const auto& obj : mGameObjects)
	{
		if (obj->getID() == id)
		{
			mGameObjectsToRemove.push_back(*obj.get());
			return;
		}
	}
	throw std::runtime_error("GameObject with ID " + std::to_string(id) + " not found.");
}

void Scene::requestGameObjectRemoval(GameObject* object)
{
	for (const auto& obj : mGameObjects)
	{
		if (obj.get() == object)
		{
			mGameObjectsToRemove.push_back(*obj.get());
			return;
		}
	}
	throw std::runtime_error("GameObject not found.");
}

GameObject& Scene::getGameObject(int id)
{
	for (const auto& obj : mGameObjects)
	{
		if (obj->getID() == id)
		{
			return *obj;
		}
	}
	throw std::runtime_error("GameObject with ID " + std::to_string(id) + " not found.");
}

std::string Scene::getName() { return mSceneName; }

int Scene::getID() { return mSceneID; }

std::vector<Camera*> Scene::getCameras() const
{
	std::vector<Camera*> cameras;
	for (const auto& obj : mGameObjects)
	{
		Camera* camera = dynamic_cast<Camera*>(obj.get());
		if (camera)
		{
			cameras.push_back(camera);
		}
	}
	return cameras;
}

Camera* Scene::getCameraWithTag(const std::string& tag) const
{
	for (const auto& obj : mGameObjects)
	{
		Camera* camera = dynamic_cast<Camera*>(obj.get());
		if (camera && camera->getTag() == tag)
		{
			return camera;
		}
	}
	return nullptr;
}

Camera* Scene::getMainCamera() const
{
	for (const auto& obj : mGameObjects)
	{
		Camera* camera = dynamic_cast<Camera*>(obj.get());
		if (camera && camera->isMainCamera())
		{
			return camera;
		}
	}
	return nullptr;
}

void Scene::addPersistentGameObject(GameObject* object)
{
	if (object)
	{
		mGameObjects.push_back(std::unique_ptr<GameObject>(object));
		mPersistentGameObjects.push_back(*object);
		EngineBravo::getInstance().getUpdateQueue().addToUpdateObjects(*object);
	}
}

/**
 * @brief Removes a GameObject from the persistent game objects list
 * @note This function does not remove the GameObject from the scene
 *
 * @param object The GameObject to remove
 */
void Scene::removePersistentGameObject(GameObject* object)
{
	for (int i = 0; i < mPersistentGameObjects.size(); i++)
	{
		if (&mPersistentGameObjects[i].get() == object)
		{
			mPersistentGameObjects.erase(mPersistentGameObjects.begin() + i);
			break;
		}
	}
}

std::vector<std::reference_wrapper<GameObject>>& Scene::getPersistentGameObjects() { return mPersistentGameObjects; }

void Scene::clearPersistentGameObjects() { mPersistentGameObjects.clear(); }

void Scene::releasePersistentGameObjects()
{
	for (auto& obj : mPersistentGameObjects)
	{
		for (int i = 0; i < mGameObjects.size(); i++)
		{
			if (mGameObjects[i].get() == &obj.get())
			{
				GameObject* warningWeg = mGameObjects[i].release();
				mGameObjects.erase(mGameObjects.begin() + i);
				break;
			}
		}
	}
}

std::vector<std::reference_wrapper<GameObject>> Scene::getGameObjectsToBeRemoved() { return mGameObjectsToRemove; }
