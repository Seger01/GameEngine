#ifndef SCENE_H
#define SCENE_H

#include "Camera.h"
#include "GameObject.h"
#include <memory>
#include <string>
#include <vector>

class Scene
{
public:
	friend class SceneManager;
	~Scene();

	void update();

	void addGameObject(GameObject* object);
	void requestGameObjectRemoval(int id);
	void requestGameObjectRemoval(GameObject* object);
	GameObject& getGameObject(int id);

	std::vector<Camera*> getCameras();
	Camera* getCameraWithTag(const std::string& tag);

	int getID();
	std::string getName();

	std::vector<GameObject*> getGameObjects();
	std::vector<GameObject*> getGameObjectsWithTag(const std::string& tag);

	void addPersistentGameObject(GameObject* object);
	std::vector<GameObject*>& getPersistentGameObjects();
	void clearPersistentGameObjects();
	void releasePersistentGameObjects();

	std::vector<GameObject*> getGameObjectsToBeRemoved();

private:
	Scene(std::string aSceneName, int aSceneID);

	void removeGameObject(GameObject* aObject);
	void removePersistentGameObject(GameObject* object);

private:
	std::vector<std::unique_ptr<GameObject>> mGameObjects;
	std::vector<GameObject*> mPersistentGameObjects;
	std::vector<GameObject*> mGameObjectsToRemove;

	int mActiveCameraIndex;
	std::string mSceneName;
	int mSceneID = -1;
};

#endif // SCENE_H
