#include "PlayerStatsBehaviourScript.h"

#include "EngineBravo.h"
#include "Input.h"

SpriteDef playerStatBackgroundDef = {"UI/ui_images.png", Rect{95, 35, 66, 26}, 99, 39};
SpriteDef playerStatHealthBarBackgroundDef = {"UI/ui_images.png", Rect{134, 19, 54, 9}, 81, 15};
SpriteDef playerStatHealthBarDef = {"UI/ui_images.png", Rect{72, 22, 49, 3}, 74, 5};

PlayerStatsBehaviourScript::PlayerStatsBehaviourScript() : mHealth(100), mMaxHealth(100), mHealthBarWidth(0) {}

void PlayerStatsBehaviourScript::onStart() {
    EngineBravo& engine = EngineBravo::getInstance();
    SceneManager& sceneManager = engine.getSceneManager();
    Scene* scene = sceneManager.getCurrentScene();
    Camera& camera = scene->getActiveCamera();

    Sprite* playerStatBackgroundSprite = engine.getResourceManager().createSprite(playerStatBackgroundDef);
    playerStatBackgroundSprite->setLayer(4);
    playerStatBackgroundSprite->setWidth(playerStatBackgroundSprite->getWidth());
    playerStatBackgroundSprite->setHeight(playerStatBackgroundSprite->getHeight());

    Sprite* playerStatHealthBarBackgroundSprite =
        engine.getResourceManager().createSprite(playerStatHealthBarBackgroundDef);
    playerStatHealthBarBackgroundSprite->setLayer(4);
    playerStatHealthBarBackgroundSprite->setWidth(playerStatHealthBarBackgroundSprite->getWidth());
    playerStatHealthBarBackgroundSprite->setHeight(playerStatHealthBarBackgroundSprite->getHeight());
    playerStatHealthBarBackgroundSprite->setRelativePosition(Transform(Vector2(39, 0)));

    // Sprite* playerStatHealthBarSprite = engine.getResourceManager().createSprite(playerStatHealthBarDef);
    // playerStatHealthBarSprite->setLayer(5);
    // playerStatHealthBarSprite->setWidth(playerStatHealthBarSprite->getWidth());
    // playerStatHealthBarSprite->setHeight(playerStatHealthBarSprite->getHeight());
    // playerStatHealthBarSprite->setRelativePosition(Transform(Vector2(42, 5)));
    // playerStatHealthBarSprite->setTag("playerHealthBar");

    Sprite* playerStatHealthBarSprite = engine.getResourceManager().createSprite(playerStatHealthBarDef);
    playerStatHealthBarSprite->setLayer(5);
    playerStatHealthBarSprite->setWidth(playerStatHealthBarSprite->getWidth());
    playerStatHealthBarSprite->setHeight(playerStatHealthBarSprite->getHeight());
    playerStatHealthBarSprite->setRelativePosition(Transform(Vector2(42, 5)));
    playerStatHealthBarSprite->setTag("playerHealthBar");

    mGameObject->addComponent(playerStatBackgroundSprite);
    mGameObject->addComponent(playerStatHealthBarBackgroundSprite);
    mGameObject->addComponent(playerStatHealthBarSprite);

    mHealthBarWidth = playerStatHealthBarSprite->getWidth();
    mFullHealthBarForegroundSourceRect = playerStatHealthBarSprite->getSource();
}

void PlayerStatsBehaviourScript::onUpdate() {
    if (mHealth > 0) {
        if (mHealth < 0) {
            mHealth = 0;
        }
        mHealth -= (10.0f * Time::deltaTime);
        if (mGameObject->getComponentsWithTag<Sprite>("playerHealthBar").size() > 0) {
            Sprite* sprite = mGameObject->getComponentsWithTag<Sprite>("playerHealthBar")[0];

            Rect sourceRect = sprite->getSource();
            sourceRect.w = mFullHealthBarForegroundSourceRect.w * (mHealth / mMaxHealth);

            if (sourceRect.w < 1) {
                sourceRect.w = 1;
            }

            sourceRect.x = mFullHealthBarForegroundSourceRect.x + mFullHealthBarForegroundSourceRect.w - sourceRect.w;
            sprite->setSource(sourceRect);

            sprite->setWidth(mHealthBarWidth * (mHealth / mMaxHealth));
        }
    } else {
        mHealth = mMaxHealth;
    }
}

void PlayerStatsBehaviourScript::onCollide(GameObject* aGameObject) {
    std::cout << "PlayerStatsBehaviourScript collided with " << aGameObject->getName() << std::endl;
}