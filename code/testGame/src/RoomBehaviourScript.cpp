#include "RoomBehaviourScript.h"
#include "BoxCollider.h"
#include "BulletPrefab.h"
#include "EnemyBehaviourScript.h"
#include "EngineBravo.h"
#include "FSConverter.h"
#include "GameObject.h"
#include "RigidBody.h"
#include "SceneManager.h"
#include "Sprite.h"
#include "SpriteDef.h"
#include <random>

void RoomBehaviourScript::onStart() {
    FSConverter fsConverter;
    std::string doorSpriteSheetPath = fsConverter.getResourcePath("Dungeontileset/atlas_walls_high-16x32.png");
    std::string enemySpritePath = fsConverter.getResourcePath("Dungeontileset/0x72_DungeonTilesetII_v1.7.png");
    const Point doorOpenPosition = {320, 80};
    const Point doorClosedPosition = {256, 80};
    mClosedDoorSpriteDef = {doorSpriteSheetPath, Rect{doorClosedPosition.x, doorClosedPosition.y, 64, 64}, 64, 64};
    mOpenDoorSpriteDef = {doorSpriteSheetPath, Rect{doorOpenPosition.x, doorOpenPosition.y, 64, 64}, 64, 64};
    mEnemyFrameDef = {enemySpritePath, Rect{182, 389, 20, 27}, 20, 27};
}

void RoomBehaviourScript::onUpdate() {
    // Check if enemy count is zero after entering
    // If so, openDoors() & set mDoorsOpen to true
}

void RoomBehaviourScript::spawnEnemies() {
    EngineBravo& engine = EngineBravo::getInstance();
    SceneManager& sceneManager = engine.getSceneManager();
    std::random_device rd;
    std::mt19937 gen(rd());

    for (const auto& spawnPoint : mEnemySpawns) {
        if (spawnPoint.roomID == mRoomID && mEnemySpawns.size() > 0) {
            std::uniform_real_distribution<> disX(spawnPoint.x, spawnPoint.x + spawnPoint.width);
            std::uniform_real_distribution<> disY(spawnPoint.y, spawnPoint.y + spawnPoint.height);

            float randomX = disX(gen);
            float randomY = disY(gen);

            GameObject* enemy = new GameObject;
            Transform transform;
            transform.position.x = randomX;
            transform.position.y = randomY;
            Sprite* enemySprite = engine.getResourceManager().createSprite(mEnemyFrameDef);
            enemySprite->setLayer(3);
            enemy->addComponent(enemySprite);

            enemy->addComponent<BoxCollider>();
            enemy->getComponents<BoxCollider>().at(0)->setWidth(enemySprite->getWidth());
            enemy->getComponents<BoxCollider>().at(0)->setHeight(enemySprite->getHeight());

            enemy->addComponent<RigidBody>();
            RigidBody* rigidBody = enemy->getComponents<RigidBody>().at(0);
            rigidBody->setHasGravity(true);
            rigidBody->setDensity(1.0f);
            rigidBody->setFriction(0.3f);
            rigidBody->setRestitution(0.2f);
            rigidBody->setMass(1.0f);
            rigidBody->setGravityScale(10.0f);
            rigidBody->setCanRotate(true);
            enemy->setTransform(transform);
            enemy->addComponent(new EnemyBehaviourScript());
            // std::cout << "Spawned enemy at (" << spawnPoint.x << ", " << spawnPoint.y << ")" << std::endl;
            sceneManager.getCurrentScene()->addGameObject(enemy);

            GameObject* bulletObject = BulletPrefabFactory().createBulletPrefab(*enemy);
            sceneManager.getCurrentScene()->addGameObject(bulletObject);
        }
    }
}

void RoomBehaviourScript::openDoors() {
    updateDoors(mOpenDoorSpriteDef);
    mDoorsOpen = true;
    // std::cout << "Doors opened" << std::endl;
}

void RoomBehaviourScript::closeDoors() {
    updateDoors(mClosedDoorSpriteDef);
    mDoorsOpen = false;
    // std::cout << "Doors closed" << std::endl;
}

void RoomBehaviourScript::updateDoors(const SpriteDef& spriteDef) {
    EngineBravo& engine = EngineBravo::getInstance();
    SceneManager& sceneManager = engine.getSceneManager();
    std::vector<GameObject*> doorGameObjects = sceneManager.getCurrentScene()->getGameObjectsWithTag("Door");
    // std::cout << "doorGameObjects: " << doorGameObjects.size() << std::endl;
    int spriteWidth = 16;
    int spriteHeight = 16;

    for (size_t i = 0; i < doorGameObjects.size(); ++i) {
        GameObject* doorPart = doorGameObjects[i];
        std::vector<Sprite*> sprites = doorPart->getComponents<Sprite>();
        for (Sprite* sprite : sprites) {
            if (sprite) {
                int index = i % 12;  // Reset index after every 12 door parts
                int col = index % 4; // 4 tiles per row
                int row = index / 4; // 4 tiles per column

                Rect sourceRect = {spriteDef.sourceRect.x + col * spriteWidth,
                                   spriteDef.sourceRect.y + row * spriteHeight, spriteWidth, spriteHeight};

                sprite->setSource(sourceRect);
            }
        }
        std::vector<BoxCollider*> bodies = doorPart->getComponents<BoxCollider>();
        if (!bodies.empty()) {
            bodies.at(0)->setActive(mDoorsOpen);
        }
        std::vector<BoxCollider*> colliders = doorPart->getComponents<BoxCollider>();
        if (!colliders.empty()) {
            colliders.at(0)->setActive(mDoorsOpen);
        }
    }
}

void RoomBehaviourScript::onCollide(GameObject* aGameObject) {
    // std::cout << "RoomBehaviourScript collided with " << aGameObject->getName() << std::endl;
    if (mDoorsOpen) {
        spawnEnemies();
        closeDoors();
    } else {
        openDoors();
    }
}
