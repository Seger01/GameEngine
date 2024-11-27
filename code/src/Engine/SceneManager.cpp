#include "Engine/SceneManager.h"
#include "EngineBravo.h"

SceneManager::SceneManager() : mCurrentSceneIndex(0), mNewSceneName(""), mNewSceneID(-1), mScenes{} {}

SceneManager::~SceneManager() {}

void SceneManager::update()
{
	if (mScenes.size() == 0)
	{
		return;
	}

	if (mCurrentSceneIndex <= mScenes.size() - 1)
	{
		mScenes[mCurrentSceneIndex]->update();
	}

	// check if scene would like to be changed
	sceneChanged();
}

bool SceneManager::sceneChanged()
{
	if (mNewSceneName != "")
	{
		loadScene(mNewSceneName);
		mNewSceneName = "";
		return true;
	}
	else if (mNewSceneID != -1)
	{
		loadScene(mNewSceneID);
		mNewSceneID = -1;
		return true;
	}
	return false;
}

void SceneManager::requestSceneChange(const std::string& sceneName) { mNewSceneName = sceneName; }

void SceneManager::requestSceneChange(int sceneID) { mNewSceneID = sceneID; }

int SceneManager::getNewSceneID()
{
	bool idFound = false;

	int sceneID = -1;
	while (idFound == false)
	{
		sceneID++;
		bool idSeen = false;
		for (int i = 0; i < mScenes.size(); i++)
		{
			if (mScenes[i]->getID() == sceneID)
			{
				idSeen = true;
			}
		}
		if (!idSeen)
			idFound = true;
	}
	return sceneID;
}

bool SceneManager::sceneNameExists(std::string aSceneName)
{
	for (auto& scene : mScenes)
	{
		if (scene->getName() == aSceneName)
		{
			return true;
		}
	}
	return false;
}

bool SceneManager::sceneIDExists(int aSceneID)
{
	for (auto& scene : mScenes)
	{
		if (scene->getID() == aSceneID)
		{
			return true;
		}
	}
	return false;
}

Scene* SceneManager::createScene(std::string aSceneName, int aSceneID)
{
	if (sceneNameExists(aSceneName) || sceneIDExists(aSceneID))
	{
		return nullptr;
	}

	if (aSceneID == -1)
	{
		aSceneID = getNewSceneID();
	}

	mScenes.push_back(std::unique_ptr<Scene>(new Scene(aSceneName, aSceneID)));
	return mScenes.back().get();
	// return mScenes[mScenes.size() - 1].get();
}

void SceneManager::removeScene(const std::string& sceneName)
{
	for (int i = 0; i < mScenes.size(); ++i)
	{
		if (mScenes[i]->getName() == sceneName)
		{
			mScenes.erase(mScenes.begin() + i);
			break;
		}
	}
}

void SceneManager::loadScene(int index)
{
	// Release all objects from the managers
	EngineBravo::getInstance().clearManagerObjects();

	Scene* currentScene = getCurrentScene();
	if (currentScene)
	{
		currentScene->releasePersistentGameObjects();
	}

	std::vector<GameObject*> persistentGameObjects = currentScene->getPersistentGameObjects();

	currentScene->clearPersistentGameObjects();

	if (index >= 0 && index < mScenes.size())
	{
		mCurrentSceneIndex = index;
	}

	currentScene = getCurrentScene();

	for (auto& object : persistentGameObjects)
	{
		currentScene->addPersistentGameObject(object);
		// Add the object to the update list
		EngineBravo::getInstance().addToUpdateObjects(*object);
	}
	EngineBravo::getInstance().updateAdditions();
}

void SceneManager::loadScene(const std::string& sceneName)
{
	// Release all objects from the managers
	EngineBravo::getInstance().clearManagerObjects();

	Scene* currentScene = getCurrentScene();
	if (currentScene)
	{
		currentScene->releasePersistentGameObjects();
	}

	std::vector<GameObject*> persistentGameObjects = currentScene->getPersistentGameObjects();

	currentScene->clearPersistentGameObjects();

	for (int i = 0; i < mScenes.size(); ++i)
	{
		if (mScenes[i]->getName() == sceneName)
		{
			std::cout << "Loading scene: " << sceneName << std::endl;
			mCurrentSceneIndex = i;
			EngineBravo::getInstance().getAudioManager().wake();
			break;
		}
	}

	currentScene = getCurrentScene();

	for (auto& object : persistentGameObjects)
	{
		currentScene->addPersistentGameObject(object);
		// Add the object to the update list
		EngineBravo::getInstance().addToUpdateObjects(*object);
	}
}

Scene* SceneManager::getCurrentScene()
{
	if (mCurrentSceneIndex >= 0 && mCurrentSceneIndex < mScenes.size())
	{
		return mScenes[mCurrentSceneIndex].get();
	}
	return nullptr;
}
