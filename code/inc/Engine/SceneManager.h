#pragma once
#include "Scene.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class SceneManager {
public:
    friend class EngineBravo;
    int addScene(const Scene& scene);
    // int addSceneLevel(const Map& map);
    void loadScene(int index);
    void loadScene(const std::string& sceneName); // Willen we dit overloaden?

    Scene* getCurrentScene();
    Scene* createScene(std::string aSceneName, int aSceneID = -1);

private:
    bool sceneNameExists(std::string aSceneName);
    bool sceneIDExists(int aSceneID);
    int getNewSceneID();

private:
    SceneManager();
    ~SceneManager();

    std::vector<std::unique_ptr<Scene>> mScenes;
    int mCurrentSceneIndex;
};
