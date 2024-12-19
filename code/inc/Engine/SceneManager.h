/**
 * @file SceneManager.h
 * @brief SceneManager class header file.
 */

#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "Scene.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>

/**
 * @class SceneManager
 * @brief Manages scenes.
 */
class SceneManager
{
public:
	friend class EngineBravo;

	void update();

	// int addSceneLevel(const Map& map);
	void requestSceneChange(const std::string& sceneName);
	void requestSceneChange(int sceneID);

	Scene& getCurrentScene();
	Scene& createScene(std::string aSceneName, int aSceneID = -1);

	void removeScene(const std::string& sceneName);

private:
	bool sceneChanged();

	void loadScene(int index);
	void loadScene(const std::string& sceneName);

	bool sceneNameExists(std::string aSceneName);
	bool sceneIDExists(int aSceneID);
	int getNewSceneID();

private:
	SceneManager();
	~SceneManager();

	/// @brief The stored scenes
	std::vector<std::unique_ptr<Scene>> mScenes;
	/// @brief The current scene index
	int mCurrentSceneIndex;

	/// @brief The new scene name after request
	std::string mNewSceneName;
	/// @brief The new scene ID after request
	int mNewSceneID;
};

#endif // SCENEMANAGER_H
