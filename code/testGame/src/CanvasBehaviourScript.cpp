#include "CanvasBehaviourScript.h"

#include "Button.h"
#include "EngineBravo.h"
#include "FPSCounterBehaviourScript.h"
#include "HelloWorldButtonBehaviour.h"
#include "PlayerStatsBehaviourScript.h"
#include "Text.h"

#include "Network/NetworkObject.h"

SpriteDef buttonSpriteDef = {"UI/ui_images.png", Rect{145, 81, 46, 14}, 46, 14};

void CanvasBehaviourScript::onTestButtonClick() { std::cout << "Test Button clicked!" << std::endl; }
void CanvasBehaviourScript::onTestButtonRelease() { std::cout << "Test Button released!" << std::endl; }

void CanvasBehaviourScript::onStart() {
    EngineBravo& engine = EngineBravo::getInstance();
    SceneManager& sceneManager = engine.getSceneManager();
    Scene* scene = sceneManager.getCurrentScene();
    Camera& camera = scene->getActiveCamera();

    Button* buttonObject = new Button;
    buttonObject->setTransform(Transform(Vector2(10, 50)));
    buttonObject->addComponent<HelloWorldButtonBehaviour>();
    buttonObject->setOnClickCallback(std::bind(&CanvasBehaviourScript::onTestButtonClick, this));
    buttonObject->setOnReleaseCallback(std::bind(&CanvasBehaviourScript::onTestButtonRelease, this));
    buttonObject->setParent(mGameObject);
    Text* buttonText = new Text("Button1", "werkt niet", Color(255, 255, 255), Vector2(0, 0), Vector2(0.5, 0.5));
    buttonText->setParent(buttonObject);

    Sprite* buttonSprite = engine.getResourceManager().createSprite(buttonSpriteDef);
    buttonObject->addComponent(buttonSprite);
    int width = 0;
    int height = 0;
    engine.getRenderSystem().getTextSize(buttonText->getFont(), buttonText->getText(), width, height,
                                         buttonText->getScale());
    width += 2;
    height += 2;
    buttonSprite->setWidth(width);
    buttonSprite->setHeight(height);
    buttonSprite->setRelativePosition(Transform(Vector2(-1, -1)));

    buttonObject->setWidth(width);
    buttonObject->setHeight(height);

    GameObject* playerStats = new GameObject;
    playerStats->addComponent<PlayerStatsBehaviourScript>();
    playerStats->setTransform(Transform(Vector2(5, camera.getHeight() - 45)));
    playerStats->setParent(mGameObject);

    scene->addGameObject(playerStats);
    scene->addGameObject(buttonObject);
    scene->addGameObject(buttonText);

    return;
}

void CanvasBehaviourScript::onUpdate() {
    // constantly retrieve the current camera and update the canvas position to the camera origin
    Camera& camera = EngineBravo::getInstance().getSceneManager().getCurrentScene()->getActiveCamera();

    Transform newTransform;
    newTransform.position = camera.getOrigin();

    mGameObject->setTransform(newTransform);
}

void CanvasBehaviourScript::onCollide(GameObject* aGameObject) {
    // std::cout << "CanvasBehaviourScript collided with " << aGameObject->getName() << std::endl;
}
