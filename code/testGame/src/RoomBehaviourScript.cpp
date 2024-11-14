#include "RoomBehaviourScript.h"
#include "SceneManager.h"
#include "EngineBravo.h"
#include "GameObject.h"
#include "SpriteDef.h"
#include "Sprite.h"
#include "BoxCollider.h"
#include "FSConverter.h"
#include "EnemyBehaviourScript.h"

void RoomBehaviourScript::onStart() {
    FSConverter fsConverter;
    std::string doorSpriteSheetPath = fsConverter.getResourcePath("Dungeontileset/atlas_walls_high-16x32.png");
    const Point doorOpenPosition = {320, 80};
    const Point doorClosedPosition = {256, 80};
    mClosedDoorSpriteDef = {doorSpriteSheetPath, Rect{doorClosedPosition.x, doorClosedPosition.y, 64, 64}, 64, 64};
    mOpenDoorSpriteDef = {doorSpriteSheetPath, Rect{doorOpenPosition.x, doorOpenPosition.y, 64, 64}, 64, 64};
}

void RoomBehaviourScript::onUpdate() {
    //Check if enemy count is zero after entering
    //If so, openDoors() & set mDoorsOpen to true
}

void RoomBehaviourScript::spawnEnemies() {
    EngineBravo& engine = EngineBravo::getInstance();
    SceneManager& sceneManager = engine.getSceneManager();
    for (const auto& spawnPoint : mEnemySpawns) {
        if (spawnPoint.roomID == mRoomID && mEnemySpawns.size() > 0) {
            GameObject* enemy = new GameObject;
            Transform transform;
            transform.position.x = spawnPoint.x;
            transform.position.y = spawnPoint.y;
            enemy->setTransform(transform);
            enemy->addComponent(new EnemyBehaviourScript());
            std::cout << "Spawned enemy at (" << spawnPoint.x << ", " << spawnPoint.y << ")" << std::endl;
            sceneManager.getCurrentScene()->addGameObject(enemy);
        }
    }
}


void RoomBehaviourScript::openDoors() {
    updateDoors(mOpenDoorSpriteDef);
    mDoorsOpen = true;
    std::cout << "Doors opened" << std::endl;
}

void RoomBehaviourScript::closeDoors() {
    updateDoors(mClosedDoorSpriteDef);
    mDoorsOpen = false;
    std::cout << "Doors closed" << std::endl;
}

void RoomBehaviourScript::updateDoors(const SpriteDef& spriteDef) {
    EngineBravo& engine = EngineBravo::getInstance();
    SceneManager& sceneManager = engine.getSceneManager();
    std::vector<GameObject*> doorGameObjects = sceneManager.getCurrentScene()->getGameObjectsWithTag("Door");
    std::cout << "doorGameObjects: " << doorGameObjects.size() << std::endl;
    int spriteWidth = 16;
    int spriteHeight = 16;

    for (size_t i = 0; i < doorGameObjects.size(); ++i) {
        GameObject* doorPart = doorGameObjects[i];
        std::vector<Sprite*> sprites = doorPart->getComponents<Sprite>();
        for (Sprite* sprite : sprites) {
            if (sprite) {
                int index = i % 12; // Reset index after every 12 door parts
                int col = index % 4; // 4 tiles per row
                int row = index / 4; // 4 tiles per column

                Rect sourceRect = {
                    spriteDef.sourceRect.x + col * spriteWidth,
                    spriteDef.sourceRect.y + row * spriteHeight,
                    spriteWidth,
                    spriteHeight
                };

                sprite->setSource(sourceRect);
            }
        }
        std::vector<BoxCollider*> colliders = doorPart->getComponents<BoxCollider>();
        if (!colliders.empty()) {
            colliders.at(0)->setActive(!mDoorsOpen);
        }
    }
}

void RoomBehaviourScript::onCollide(GameObject* aGameObject) {
    std::cout << "RoomBehaviourScript collided with " << aGameObject->getName() << std::endl;
    if (mDoorsOpen) {
        spawnEnemies();
        closeDoors();
        
    }
    else {
        openDoors();
    }
}
