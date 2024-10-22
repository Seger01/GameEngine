#pragma once
#include "Scene.h"
#include <iostream>
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

private:
    SceneManager();
    ~SceneManager();

    // std::vector<Scene&> scenes;
    int currentSceneIndex;
};
