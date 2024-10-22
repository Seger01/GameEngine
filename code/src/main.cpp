#include <iostream>

#include "Engine/EngineBravo.h"
#include "Components/Sprite.h"
#include "test.h"

int main() {
    EngineBravo engine;
    engine.initizalize();

    SceneManager& sceneManager = engine.getSceneManager();

    GameObject* gameObject = new GameObject;

    gameObject->addComponent<Sprite>();

    gameObject->getComponent<Sprite>()->setFlipX(true);

    std::cout << "flipX: " << gameObject->getComponent<Sprite>()->getFlipX() << std::endl;

    engine.run();

    return 0;
}
