#include "Scene.h"
#include <algorithm>
#include <stdexcept>

Scene::Scene(std::string aSceneName, int aSceneID)
    : mSceneName(aSceneName), mSceneID(aSceneID), mActiveCameraIndex(-1) {}

Scene::~Scene() {
    // Clean up GameObject pointers
    for (GameObject* obj : mGameObjects) {
        delete obj;
    }
    mGameObjects.clear();
}

std::vector<GameObject*>& Scene::getGameObjects() { return mGameObjects; }

void Scene::addGameObject(GameObject* object) {
    if (object) {
        mGameObjects.push_back(object);
    }
}

void Scene::removeGameObject(int id) {
    for (auto it = mGameObjects.begin(); it != mGameObjects.end(); ++it) {
        if ((*it)->getID() == id) {
            delete *it;
            mGameObjects.erase(it);
            return;
        }
    }
    throw std::runtime_error("GameObject with ID " + std::to_string(id) + " not found.");
}

void Scene::removeGameObject(GameObject* object) {
    auto it = std::find(mGameObjects.begin(), mGameObjects.end(), object);
    if (it != mGameObjects.end()) {
        delete *it;
        mGameObjects.erase(it);
    }
}

GameObject& Scene::getGameObject(int id) {
    for (GameObject* obj : mGameObjects) {
        if (obj->getID() == id) {
            return *obj;
        }
    }
    throw std::runtime_error("GameObject with ID " + std::to_string(id) + " not found.");
}

std::string Scene::getName() { return mSceneName; }
int Scene::getID() { return mSceneID; }

int Scene::addCamera() {
    Camera newCamera;
    mCameras.push_back(newCamera);
    return mCameras.size() - 1; // Return the index of the newly added camera
}

void Scene::removeCamera(int id) {
    if (id >= 0 && id < mCameras.size()) {
        mCameras.erase(mCameras.begin() + id);
        // Adjust active camera if necessary
        if (mActiveCameraIndex == id) {
            mActiveCameraIndex = -1; // No active camera
        } else if (mActiveCameraIndex > id) {
            mActiveCameraIndex--;
        }
    } else {
        throw std::runtime_error("Camera with ID " + std::to_string(id) + " not found.");
    }
}

Camera& Scene::getCamera(int id) {
    if (id >= 0 && id < mCameras.size()) {
        return mCameras[id];
    }
    throw std::runtime_error("Camera with ID " + std::to_string(id) + " not found.");
}

void Scene::setActiveGamera(int id) {
    if (id >= 0 && id < mCameras.size()) {
        mActiveCameraIndex = id;
    } else {
        throw std::runtime_error("Camera with ID " + std::to_string(id) + " not found.");
    }
}

Camera& Scene::getActiveCamera() {
    if (mActiveCameraIndex >= 0 && mActiveCameraIndex < mCameras.size()) {
        return mCameras[mActiveCameraIndex];
    }
    throw std::runtime_error("No active camera set.");
}
