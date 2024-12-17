#ifndef SCENE_H
#define SCENE_H

#include "Camera.h"
#include "GameObject.h"
#include <functional>
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

	std::vector<Camera*> getCameras() const;
	Camera* getCameraWithTag(const std::string& tag) const;
	Camera* getMainCamera() const;

	int getID();
	std::string getName();

	std::vector<std::reference_wrapper<GameObject>> getGameObjects() const;
	std::vector<std::reference_wrapper<GameObject>> getGameObjectsWithTag(const std::string& tag);

	void addPersistentGameObject(GameObject* object);
	std::vector<std::reference_wrapper<GameObject>>& getPersistentGameObjects();
	void clearPersistentGameObjects();
	void releasePersistentGameObjects();

	std::vector<std::reference_wrapper<GameObject>> getGameObjectsToBeRemoved();

private:
	Scene(std::string aSceneName, int aSceneID);

	void removeGameObject(GameObject* aObject);
	void removePersistentGameObject(GameObject* object);

private:
	std::vector<std::unique_ptr<GameObject>> mGameObjects;
	std::vector<std::reference_wrapper<GameObject>> mPersistentGameObjects;
	std::vector<std::reference_wrapper<GameObject>> mGameObjectsToRemove;

	int mActiveCameraIndex;
	std::string mSceneName;
	int mSceneID = -1;
};

#endif // SCENE_H
