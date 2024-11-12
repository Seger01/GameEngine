#include "RoomBehaviourScript.h"
#include "SceneManager.h"
#include "EngineBravo.h"
#include "GameObject.h"
#include "SpriteDef.h"
#include "Sprite.h"
#include "BoxCollider.h"

void RoomBehaviourScript::onStart() {
    
}

void RoomBehaviourScript::onUpdate() {
    //Check if enemy count is zero after entering
    //If so, openDoors() & set mDoorsOpen to true
}

void RoomBehaviourScript::onTriggerEnter(GameObject* aPlayer) {
    std::cout << "Player entered room: " << mRoomID << std::endl;
    mDoorsOpen = false;
    spawnEnemies();
    closeDoors();
    //Remove collider for room trigger
}

void RoomBehaviourScript::spawnEnemies() {
    EngineBravo& engine = EngineBravo::getInstance();
    SceneManager& sceneManager = engine.getSceneManager();
    for (const auto& spawnPoint : mEnemySpawns) {
        GameObject* enemy = new GameObject;
        Transform transform;
        transform.position.x = spawnPoint.x;
        transform.position.y = spawnPoint.y;
        enemy->setTransform(transform);
        // enemy->addComponent(new EnemyBehaviourScript());
        std::cout << "Spawned enemy at (" << spawnPoint.x << ", " << spawnPoint.y << ")" << std::endl;
        sceneManager.getCurrentScene()->addGameObject(enemy);
    }
}


void RoomBehaviourScript::openDoors() {
    updateDoors(mOpenDoorSpriteDef);
    std::cout << "Doors opened" << std::endl;
}

void RoomBehaviourScript::closeDoors() {
    updateDoors(mClosedDoorSpriteDef);
    std::cout << "Doors closed" << std::endl;
}
void RoomBehaviourScript::updateDoors(const SpriteDef& spriteDef) {
    EngineBravo& engine = EngineBravo::getInstance();
    SceneManager& sceneManager = engine.getSceneManager();
    std::vector<GameObject*> doorGameObjects = sceneManager.getCurrentScene()->getGameObjectsWithTag("Door");

    int spriteWidth = spriteDef.width / 4;
    int spriteHeight = spriteDef.height / 4;

    for (size_t i = 0; i < doorGameObjects.size(); ++i) {
        GameObject* doorPart = doorGameObjects[i];
        std::vector<Sprite*> sprites = doorPart->getComponents<Sprite>();
        for (Sprite* sprite : sprites) {
            if (sprite) {
                int col = i % 4;
                int row = i / 4;
                Rect sourceRect = {spriteDef.sourceRect.x + col * spriteWidth, spriteDef.sourceRect.y + row * spriteHeight, spriteWidth, spriteHeight};
                sprite->setSource(sourceRect);
            }
        }
        std::vector<BoxCollider*> colliders = doorPart->getComponents<BoxCollider>();
        if (colliders.empty()) {
            continue;
        }
        for (BoxCollider* collider : colliders) {
            if (collider) {
                if (!mDoorsOpen) {
                    collider->setActive(true);
                }
                else {
                    collider->setActive(false);
                }
            }
        }
    }
}