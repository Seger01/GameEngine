#include "Engine/SceneManager.h"

SceneManager::SceneManager() : currentSceneIndex(-1) {
}

SceneManager::~SceneManager() {
}

int SceneManager::addScene(const Scene& scene) {
    scenes.push_back(scene);
    if (currentSceneIndex == -1) {
        currentSceneIndex = 0;
    }
    return scenes.size() - 1;
}

void SceneManager::loadScene(int index) {
    if (index >= 0 && index < scenes.size()) {
        currentSceneIndex = index;
    }
}

void SceneManager::loadScene(const std::string& sceneName) {
    for (int i = 0; i < scenes.size(); ++i) {
        if (scenes[i].getName() == sceneName) {
            currentSceneIndex = i;
            break;
        }
    }
}

Scene* SceneManager::getCurrentScene() {
    if (currentSceneIndex >= 0 && currentSceneIndex < scenes.size()) {
        return &scenes[currentSceneIndex];
    }
    return nullptr;
}

