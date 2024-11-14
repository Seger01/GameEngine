#ifndef SCENE_H
#define SCENE_H

#include "Camera.h"
#include "GameObject.h"
#include <memory>
#include <string>
#include <vector>

class Scene {
public:
    friend class SceneManager;
    ~Scene();
    void addGameObject(GameObject* object);
    void removeGameObject(int id);
    void removeGameObject(GameObject* object);
    GameObject& getGameObject(int id);

    int addCamera();
    void removeCamera(int id);
    Camera& getCamera(int id);
    void setActiveCamera(int id);
    Camera& getActiveCamera();

    int getID();
    std::string getName();

    std::vector<GameObject*>& getGameObjects();
    std::vector<GameObject*> getGameObjectsWithTag(const std::string& tag);

    void addPersistentGameObject(GameObject* object);
    void removePersistentGameObject(GameObject* object);
    std::vector<GameObject*>& getPersistentGameObjects();
    void clearPersistentGameObjects();

    void releasePersistentGameObjects();

private:
    Scene(std::string aSceneName, int aSceneID);

private:
    std::vector<std::unique_ptr<GameObject>> mGameObjects;
    std::vector<GameObject*> mPersistentGameObjects;

    std::vector<std::unique_ptr<Camera>> mCameras;
    int mActiveCameraIndex;
    std::string mSceneName;
    int mSceneID = -1;
};

#endif // SCENE_H
