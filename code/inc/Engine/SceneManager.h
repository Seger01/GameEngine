#pragma once
#include "Scene.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class SceneManager {
public:
    friend class EngineBravo;

    bool sceneChanged();

    int addScene(const Scene& scene);
    // int addSceneLevel(const Map& map);
    void requestSceneChange(const std::string& sceneName);
    void requestSceneChange(int sceneID);

    Scene* getCurrentScene();
    Scene* createScene(std::string aSceneName, int aSceneID = -1);

    void removeScene(const std::string& sceneName);

private:
    void loadScene(int index);
    void loadScene(const std::string& sceneName); // Willen we dit overloaden?

    bool sceneNameExists(std::string aSceneName);
    bool sceneIDExists(int aSceneID);
    int getNewSceneID();

private:
    SceneManager();
    ~SceneManager();

    std::vector<std::unique_ptr<Scene>> mScenes;
    int mCurrentSceneIndex;

    std::string mNewSceneName;
    int mNewSceneID;
};
