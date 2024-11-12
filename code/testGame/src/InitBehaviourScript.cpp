#include "InitBehaviourScript.h"

#include "Button.h"
#include "CanvasBehaviourScript.h"
#include "EngineBravo.h"
#include "FPSCounterBehaviourScript.h"
#include "RoomBehaviourScript.h"
#include "FSConverter.h"
#include "PlayerBehaviourScript.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Text.h"
#include "TileMapParser.h"
#include "BoxCollider.h"

void InitBehaviourScript::createLevel1() {
    EngineBravo& engine = EngineBravo::getInstance();
    SceneManager& sceneManager = engine.getSceneManager();

    Scene* scene = sceneManager.createScene("Level-1");
    if (scene == nullptr)
        exit(1);

    int cameraID = scene->addCamera();
    scene->setActiveCamera(cameraID);

    scene->getActiveCamera().setTransform(Transform(Vector2(80, 96)));
    scene->getActiveCamera().setWidth(16 * 30);
    scene->getActiveCamera().setHeight(9 * 30);


    FSConverter fsConverter;
    std::string path = fsConverter.getResourcePath("LevelDefs/levelwithcollision.json");

    TileMapParser tileMapParser(path);
    tileMapParser.parse();
    const TileMapData& tileMapData = tileMapParser.getTileMapData();
    scene->getActiveCamera().setTransform(Transform(Vector2(80, 96)));
    scene->getActiveCamera().setWidth(16 * 30);
    scene->getActiveCamera().setHeight(9 * 30);

    for (const auto& roomTrigger : tileMapData.mRoomTriggers) {
        std::cout << "Parsed Room Trigger: " << roomTrigger.roomID
                  << " at (" << roomTrigger.x << ", " << roomTrigger.y
                  << ") with dimensions (" << roomTrigger.mWidth << ", " << roomTrigger.mHeight << ")" << std::endl;

        // Collect enemy spawns for this room
        std::vector<SpawnPoint> enemySpawns;
        for (const auto& spawnPoint : tileMapData.mSpawnPoints) {
            if (spawnPoint.isEnemySpawn && spawnPoint.roomID == roomTrigger.roomID) {
                enemySpawns.push_back(spawnPoint);
            }
        }  

        std::string doorSpriteSheetPath = fsConverter.getResourcePath("Dungeontileset/atlas_walls_high-16x32.png");
        const Point doorOpenPosition = {336, 96};
        const Point doorClosedPosition = {272, 96};
        SpriteDef closedDoorSpriteDef = {doorSpriteSheetPath, Rect{doorClosedPosition.x, doorClosedPosition.y, 64, 64}, 64, 64};
        SpriteDef openDoorSpriteDef = {doorSpriteSheetPath, Rect{doorOpenPosition.x, doorOpenPosition.y, 64, 64}, 64, 64};
        GameObject* roomObject = new GameObject;
        roomObject->addComponent(new RoomBehaviourScript(roomTrigger.roomID, enemySpawns, closedDoorSpriteDef, openDoorSpriteDef));
        BoxCollider* boxCollider = new BoxCollider();
        Transform transform;
        transform.position.x = roomTrigger.x;
        transform.position.y = roomTrigger.y;
        boxCollider->setTransform(transform);
        boxCollider->setWidth(roomTrigger.mWidth);
        boxCollider->setHeight(roomTrigger.mHeight);
        roomObject->addComponent(boxCollider);
        scene->addGameObject(roomObject);
    }

    GameObject* gameObject = new GameObject;

    Transform objectTransform;
    //objectTransform.position.x = 80;
    //objectTransform.position.y = 100;
    for (const auto& spawnPoint : tileMapData.mSpawnPoints) {
        if (spawnPoint.isPlayerSpawn) {
            objectTransform.position.x = spawnPoint.x;
            objectTransform.position.y = spawnPoint.y;
            break;
        }
    }
    gameObject->setTransform(objectTransform);

    gameObject->addComponent<PlayerBehaviourScript>();

    scene->addGameObject(gameObject);

    GameObject* canvasObject = new GameObject;

    canvasObject->addComponent<CanvasBehaviourScript>();

    scene->addGameObject(canvasObject);

    sceneManager.requestSceneChange("Level-1");


    // tileMapParser.printLayers();



    // // print mTileInfoMap
    // for (const auto& pair : tileMapData.mTileInfoMap) {
    //     int gID = pair.first;
    //     const TileInfo& info = pair.second;
    //     std::cout << "gID: " << gID << ", Tileset: " << info.mTilesetName << ", Coordinates: ("
    //               << info.mCoordinates.first << ", " << info.mCoordinates.second << ")" << std::endl;
    // }

    // Assuming tileMapData is a const reference to TileMapData
    for (size_t layerIndex = 0; layerIndex < tileMapData.mLayers.size(); ++layerIndex) {
        bool isDoorsLayer = (tileMapData.mLayerNames[layerIndex] == "Doors");
        // Access rows within the layer by index
        for (size_t rowIndex = 0; rowIndex < tileMapData.mLayers[layerIndex].size(); ++rowIndex) {
            // Access each tile in the row by index
            for (size_t colIndex = 0; colIndex < tileMapData.mLayers[layerIndex][rowIndex].size(); ++colIndex) {
                int tile = tileMapData.mLayers[layerIndex][rowIndex][colIndex];
                if (tile != 0) {
                    // Check if the tile exists in mTileInfoMap (read-only)
                    auto it = tileMapData.mTileInfoMap.find(tile);
                    if (it != tileMapData.mTileInfoMap.end()) {
                        const TileInfo& tileInfo = it->second; // Access as const

                        SpriteDef spriteDef = {tileInfo.mTilesetName,
                                               Rect{tileInfo.mCoordinates.first, tileInfo.mCoordinates.second, 16, 16},
                                               16, 16};

                        GameObject* gameObject = new GameObject;

                        Transform objectTransform;
                        objectTransform.position.x = static_cast<int>(colIndex * 16);
                        objectTransform.position.y = static_cast<int>(rowIndex * 16);
                        gameObject->setTransform(objectTransform);

                        // Add a Sprite component to the GameObject
                        Sprite* sprite = engine.getResourceManager().createSprite(spriteDef);

                        sprite->setLayer(layerIndex);

                        gameObject->addComponent(sprite);

                        // Add BoxCollider components to the GameObject
                        for (const auto& collider : tileInfo.mColliders) {
                            BoxCollider* boxCollider = new BoxCollider();
                            Transform transform;
                            transform.position.x = collider.x;
                            transform.position.y = collider.y;
                            boxCollider->setTransform(transform);
                            boxCollider->setWidth(collider.mWidth);
                            boxCollider->setHeight(collider.mHeight);
                            if(isDoorsLayer) {
                                gameObject->setTag("Door");
                                boxCollider->setActive(false);
                            }
                            gameObject->addComponent(boxCollider);
                        }

                        scene->addGameObject(gameObject);

                    } else {
                        // Handle the case where tileId does not exist in the map
                        std::cout << "Tile ID " << tile << " not found in mTileInfoMap.\n";
                    }
                }
            }
        }
    }

    return;
}

void InitBehaviourScript::onStart() {
    createLevel1();
    // std::cout << "InitBehaviourScript::onStart()" << std::endl;
    // EngineBravo& engine = EngineBravo::getInstance();
    // SceneManager& sceneManager = engine.getSceneManager();

    // Scene* scene = sceneManager.createScene("Level-1");
    // if (scene == nullptr)
    //     exit(1);
    //
    // int cameraID = scene->addCamera();
    // scene->setActiveGamera(cameraID);
    //
    // scene->getActiveCamera().setTransform(Transform(Vector2(80, 0)));
    // scene->getActiveCamera().setWidth(160);
    // scene->getActiveCamera().setHeight(90);
    //
    // GameObject* gameObject = new GameObject;
    //
    // Transform objectTransform;
    // objectTransform.position.x = 80;
    // objectTransform.position.y = 0;
    // gameObject->setTransform(objectTransform);
    //
    // gameObject->addComponent<PlayerBehaviourScript>();
    //
    // scene->addGameObject(gameObject);
    //
    // sceneManager.requestSceneChange("Level-1");
}

void InitBehaviourScript::onUpdate() {}
