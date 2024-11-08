#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "EngineBravo.h"

#include "InitBehaviourScript.h"

void engineTest() {
    EngineBravo& engine = EngineBravo::getInstance();
    SceneManager& sceneManager = engine.getSceneManager();

    NetworkManager& networkManager = engine.getNetworkManager();

    // networkManager.setDefaultPlayerPrefab(defaultPlayerPrefab);

    std::string role;
    std::cout << "Do you want to be a server or a client? ";
    std::cin >> role;
    bool roleSet = false;

    while (!roleSet) {
        if (role == "server") {
            roleSet = true;
            networkManager.setRole(NetworkRole::SERVER);
        } else if (role == "client") {
            roleSet = true;
            networkManager.setRole(NetworkRole::CLIENT);
        } else {
            std::cerr << "Invalid input. Please enter 'server' or 'client'." << std::endl;
            std::cin >> role;
        }
    }

    Scene* scene = sceneManager.createScene("initscene");
    if (scene == nullptr)
        exit(1);

    int cameraID = scene->addCamera();
    scene->setActiveCamera(cameraID);

    scene->getActiveCamera().setTransform(Transform(Vector2(80, 0)));
    scene->getActiveCamera().setWidth(160);
    scene->getActiveCamera().setHeight(90);

    GameObject* gameObject = new GameObject;

    gameObject->addComponent<InitBehaviourScript>();

    scene->addGameObject(gameObject);

    sceneManager.requestSceneChange("initscene");

    engine.initialize();
    engine.run();

    return;
}

int main() {
    engineTest();

    return 0;
}