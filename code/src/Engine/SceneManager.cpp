#include "Engine/SceneManager.h"
#include "Engine/EngineBravo.h"
#include <functional>

/**
 * @brief Construct a new Scene Manager:: Scene Manager object
 */
SceneManager::SceneManager() : mCurrentSceneIndex(-1), mNewSceneName(""), mNewSceneID(-1), mScenes{} {}

/**
 * @brief Destroy the Scene Manager:: Scene Manager object
 */
SceneManager::~SceneManager() {}

/**
 * @brief Update the scene manager
 */
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

/**
 * @brief Check if the scene has changed
 * @return true if the scene has changed
 * @return false if the scene has not changed
 */
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

/**
 * @brief Request a scene change
 * @param sceneName The name of the scene to change to
 */
void SceneManager::requestSceneChange(const std::string& sceneName) { mNewSceneName = sceneName; }

/**
 * @brief Request a scene change
 * @param sceneID The ID of the scene to change to
 */
void SceneManager::requestSceneChange(int sceneID) { mNewSceneID = sceneID; }

/**
 * @brief Get a new scene ID
 * @return int The new scene ID
 */
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

/**
 * @brief Check if a scene name exists
 * @param aSceneName The name of the scene to check
 * @return true if the scene name exists
 * @return false if the scene name does not exist
 */
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

/**
 * @brief Check if a scene ID exists
 * @param aSceneID The ID of the scene to check
 * @return true if the scene ID exists
 * @return false if the scene ID does not exist
 */
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

/**
 * @brief Create a scene
 * @param aSceneName The name of the scene to create
 * @param aSceneID The ID of the scene to create
 * @return Scene& The created scene
 */
Scene& SceneManager::createScene(std::string aSceneName, int aSceneID)
{
	if (sceneNameExists(aSceneName) || sceneIDExists(aSceneID))
	{
		throw std::runtime_error("Scene name or ID already exists");
	}

	if (aSceneID == -1)
	{
		aSceneID = getNewSceneID();
	}

	mScenes.push_back(std::unique_ptr<Scene>(new Scene(aSceneName, aSceneID)));
	return *mScenes[mScenes.size() - 1];
}

/**
 * @brief Remove a scene
 * @param sceneName The name of the scene to remove
 */
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

/**
 * @brief Load a scene
 * @param index The index of the scene to load
 */
void SceneManager::loadScene(int index)
{
	// Release all objects from the managers
	EngineBravo::getInstance().getUpdateQueue().clearManagerObjects();

	Scene* currentScene = &getCurrentScene();
	if (currentScene)
	{
		currentScene->releasePersistentGameObjects();
	}

	std::vector<std::reference_wrapper<GameObject>> persistentGameObjects = currentScene->getPersistentGameObjects();

	currentScene->clearPersistentGameObjects();

	for (int i = 0; i < mScenes.size(); ++i)
	{
		if (mScenes[i]->getID() == index)
		{
			std::cout << "Loading scene with ID: " << index << std::endl;
			mCurrentSceneIndex = i;
			EngineBravo::getInstance().getAudioManager().wake();
			break;
		}
	}

	currentScene = &getCurrentScene();

	for (auto& object : persistentGameObjects)
	{
		currentScene->addPersistentGameObject(&object.get());
		// Add the object to the update list
		EngineBravo::getInstance().getUpdateQueue().addToUpdateObjects(object);
	}
	for (GameObject& object : currentScene->getGameObjects())
	{
		EngineBravo::getInstance().getUpdateQueue().addToUpdateObjects(object);
	}
	EngineBravo::getInstance().getUpdateQueue().updateAdditions();
}

/**
 * @brief Load a scene
 * @param sceneName The name of the scene to load
 */
void SceneManager::loadScene(const std::string& sceneName)
{
	if (mScenes.size() == 0)
	{
		throw std::runtime_error("Requested to load scene but no scene exist");
	}

	// Release all objects from the managers
	EngineBravo::getInstance().getUpdateQueue().clearManagerObjects();

	Scene* currentScene = &getCurrentScene();
	if (currentScene)
	{
		currentScene->releasePersistentGameObjects();
	}

	std::vector<std::reference_wrapper<GameObject>> persistentGameObjects = currentScene->getPersistentGameObjects();

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
		if (i == mScenes.size() - 1)
		{
			// thow runtime error if scene does not exist but tell the user which scene name was incorrect
			throw std::runtime_error("Scene name does not exist: " + sceneName);
		}
	}

	currentScene = &getCurrentScene();

	for (auto& object : persistentGameObjects)
	{
		currentScene->addPersistentGameObject(&object.get());
		// Add the object to the update list
		EngineBravo::getInstance().getUpdateQueue().addToUpdateObjects(object);
	}
	for (GameObject& object : currentScene->getGameObjects())
	{
		EngineBravo::getInstance().getUpdateQueue().addToUpdateObjects(object);
	}
	EngineBravo::getInstance().getUpdateQueue().updateAdditions();
}

/**
 * @brief Get the current scene
 * @return Scene& The current scene
 */
Scene& SceneManager::getCurrentScene()
{
	if (mCurrentSceneIndex <= -1)
	{
		static Scene emptyScene("Non valid scene", -1);
		return emptyScene;
	}

	if (mCurrentSceneIndex >= 0 && mCurrentSceneIndex < mScenes.size())
	{
		return *mScenes[mCurrentSceneIndex];
	}

	throw std::runtime_error("Scene index out of bounds");
}
