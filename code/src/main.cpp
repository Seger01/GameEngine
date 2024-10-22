#include <iostream>

#include "Components/Sprite.h"
#include "Engine/EngineBravo.h"
#include "Engine/SceneManager.h"
#include "Scene.h"
#include "test.h"

int main() {
    EngineBravo engine;
    engine.initizalize();

    SceneManager& sceneManager = engine.getSceneManager();
    Scene scene = Scene();
    sceneManager.addScene(scene);

    GameObject* gameObject = new GameObject;

    gameObject->addComponent<Sprite>();
    gameObject->addComponent<SampleBehaviourScript>();

    gameObject->getComponent<Sprite>()->setFlipX(true);

    scene.addGameObject(gameObject);

    sceneManager.loadScene(0);

    std::cout << "flipX: " << gameObject->getComponent<Sprite>()->getFlipX() << std::endl;

    engine.run();

    return 0;
}
