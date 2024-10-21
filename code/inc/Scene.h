#pragma once
#include "GameObject.h"
#include "Camera.h"
#include <string>
#include <vector>

class Scene
{
    public:
        Scene();
        ~Scene();
        void addGameObject(GameObject object);
        void removeGameObject(int id);
        GameObject& getGameObject(int id);
        int addCamera();
        void removeCamera(int id);
        Camera& getCamera(int id);
        void setActiveGamera(int id);
        Camera& getActiveCamera();
    private:
        std::vector<GameObject> mGameObjects; //Object of pointer?
        std::vector<Camera> mCameras;         //Object of pointer?
        int mActiveCameraIndex;
        std::string sceneName;
};