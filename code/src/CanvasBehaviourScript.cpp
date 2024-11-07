#include "CanvasBehaviourScript.h"

#include "Button.h"
#include "EngineBravo.h"

SpriteDef buttonSpriteDef = {"UI/ui_images.png", Rect{0, 287, 64, 16}, 64, 16};

void CanvasBehaviourScript::onStart() {
    EngineBravo& engine = EngineBravo::getInstance();
    SceneManager& sceneManager = engine.getSceneManager();
    Scene* scene = sceneManager.getCurrentScene();

    Button* buttonObject = new Button;
    buttonObject->setTransform(Transform(Vector2(20, 20)));

    Sprite* sprite = engine.getResourceManager().createSprite(buttonSpriteDef);
    sprite->setLayer(4);
    buttonObject->addComponent(sprite);

    buttonObject->setWidth(sprite->getWidth());
    buttonObject->setHeight(sprite->getHeight());

    buttonObject->setParent(mGameObject);

    scene->addGameObject(buttonObject);

    return;
}

void CanvasBehaviourScript::onUpdate() {
    // constantly retrieve the current camera and update the canvas position to the camera origin
    Camera& camera = EngineBravo::getInstance().getSceneManager().getCurrentScene()->getActiveCamera();

    Transform newTransform;
    newTransform.position = camera.getOrigin();

    mGameObject->setTransform(newTransform);
}
