#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "EngineBravo.h"

#include "Network/NetworkTransform.h"
#include "PlayerPrefab.h"
#include "StartupBehaviourScript.h"

void engineTest() {
    EngineBravo& engine = EngineBravo::getInstance();
    engine.initialize();
    SceneManager& sceneManager = engine.getSceneManager();

    NetworkManager& networkManager = engine.getNetworkManager();

    GameObject* defaultPlayerPrefab = PlayerPrefabFactory().createPlayerPrefab();

    networkManager.setDefaultPlayerPrefab(defaultPlayerPrefab);

    Scene* sceneStartup = sceneManager.createScene("startupScene");

    int cameraIDStartup = sceneStartup->addCamera();
    sceneStartup->setActiveCamera(cameraIDStartup);

    sceneStartup->getActiveCamera().setTransform(Transform(Vector2(50, 50)));
    sceneStartup->getActiveCamera().setWidth(100);
    sceneStartup->getActiveCamera().setHeight(100);

    GameObject* gameObjectStartup = new GameObject;
    SpriteDef logo = {"Engine_logo/Engine-BRAVO_logo.png", Rect(), 100, 100};
    Sprite* sprite = EngineBravo::getInstance().getResourceManager().createSprite(logo);

    gameObjectStartup->addComponent(sprite);

    sceneStartup->addGameObject(gameObjectStartup);

    GameObject* startupBehaviourObject = new GameObject;
    startupBehaviourObject->addComponent<StartupBehaviourScript>();
    sceneStartup->addGameObject(startupBehaviourObject);

    sceneManager.requestSceneChange("startupScene");

    engine.run();

    return;
}

int main() {
    engineTest();

    return 0;
}