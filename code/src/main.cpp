#include <iostream>

#include "Components/Sprite.h"
#include "Engine/EngineBravo.h"
#include "Engine/SceneManager.h"
#include "SampleBevahiourScript.h"
#include "Scene.h"
#include "test.h"

int main() {
    EngineBravo engine;
    engine.initizalize();
    SceneManager& sceneManager = engine.getSceneManager();

    Scene* scene = sceneManager.createScene("Level1");
    if (scene == nullptr)
        exit(1);

    GameObject* gameObject = new GameObject;

    gameObject->addComponent<Sprite>();
    gameObject->addComponent<SampleBehaviourScript>();
    gameObject->getComponent<Sprite>()->setFlipX(true);

    scene->addGameObject(gameObject);

    sceneManager.loadScene(0);

    // std::cout << "flipX: " << gameObject->getComponent<Sprite>()->getFlipX() << std::endl;

    engine.run();

    return 0;
}
