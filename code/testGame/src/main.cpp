#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "EngineBravo.h"

#include "Network/NetworkTransform.h"
#include "NetworkBehaviourScript.h"
#include "PlayerBehaviourScript.h"

void engineTest() {
    EngineBravo& engine = EngineBravo::getInstance();
    SceneManager& sceneManager = engine.getSceneManager();

    NetworkManager& networkManager = engine.getNetworkManager();

    GameObject* defaultPlayerPrefab = new GameObject;

    Transform objectTransform;
    objectTransform.position.x = 80;
    objectTransform.position.y = 100;
    defaultPlayerPrefab->setTransform(objectTransform);
    defaultPlayerPrefab->addComponent<PlayerBehaviourScript>();
    defaultPlayerPrefab->setTag("Player");
    defaultPlayerPrefab->addComponent<NetworkTransform>(true, true, false, false, false);
    networkManager.setDefaultPlayerPrefab(defaultPlayerPrefab);

    Scene* scene = sceneManager.createScene("networkScene");
    if (scene == nullptr)
        exit(1);

    int cameraID = scene->addCamera();
    scene->setActiveCamera(cameraID);

    scene->getActiveCamera().setTransform(Transform(Vector2(100, 100)));
    scene->getActiveCamera().setWidth(200);
    scene->getActiveCamera().setHeight(200);

    GameObject* gameObject = new GameObject;

    gameObject->addComponent<NetworkBehaviourScript>();

    scene->addGameObject(gameObject);

    sceneManager.requestSceneChange("networkScene");

    engine.initialize();
    engine.run();

    return;
}

int main() {
    engineTest();

    return 0;
}