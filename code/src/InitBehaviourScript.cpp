#include "InitBehaviourScript.h"

#include "BoxCollider.h"
#include "EngineBravo.h"
#include "FSConverter.h"
#include "GameObject.h"
#include "PlayerBehaviourScript.h"
#include "RigidBody.h"
#include "Scene.h"
#include "SceneManager.h"
#include "TileMapParser.h"
#include "Transform.h"

SpriteDef guyFrameDef = {"Dungeontileset/0x72_DungeonTilesetII_v1.7.png", Rect{182, 389, 20, 27}, 20, 27};

void InitBehaviourScript::createLevel1() {
    EngineBravo& engine = EngineBravo::getInstance();
    SceneManager& sceneManager = engine.getSceneManager();

    Scene* scene = sceneManager.createScene("Level-1");
    if (scene == nullptr)
        exit(1);

    int cameraID = scene->addCamera();
    scene->setActiveGamera(cameraID);

    scene->getActiveCamera().setTransform(Transform(Vector2(80, 100)));
    scene->getActiveCamera().setWidth(160 * 3);
    scene->getActiveCamera().setHeight(90 * 3);

    GameObject* gameObject = new GameObject;

    Transform objectTransform;
    objectTransform.position.x = 80;
    objectTransform.position.y = 100;
    gameObject->setTransform(objectTransform);

    gameObject->addComponent<PlayerBehaviourScript>();

    scene->addGameObject(gameObject);

    sceneManager.requestSceneChange("Level-1");

    GameObject* gameObject2 = new GameObject;
    Transform objectTransform2;
    objectTransform2.position.x = 50;
    objectTransform2.position.y = 80;
    gameObject2->setTransform(objectTransform2);

    Sprite* guySprite = engine.getResourceManager().createSprite(guyFrameDef);
    guySprite->setLayer(3);
    gameObject2->addComponent(guySprite);

    gameObject2->addComponent<BoxCollider>();

    gameObject2->getComponents<BoxCollider>().at(0)->setWidth(guySprite->getWidth());
    gameObject2->getComponents<BoxCollider>().at(0)->setHeight(guySprite->getHeight());

    gameObject2->addComponent<RigidBody>();
    RigidBody* rigidBody = gameObject2->getComponents<RigidBody>().at(0);
    rigidBody->setHasGravity(true);
    rigidBody->setDensity(1.0f);
    rigidBody->setFriction(0.3f);
    rigidBody->setRestitution(0.2f);
    rigidBody->setMass(1.0f);
    rigidBody->setGravityScale(10.0f);
    rigidBody->setCanRotate(false);
    gameObject2->setName("Guy");
    gameObject2->setTag("Guy");
    gameObject2->setID(1);

    scene->addGameObject(gameObject2);

    FSConverter fsConverter;
    std::string path = fsConverter.getResourcePath("LevelDefs/levelwithcollision.json");

    TileMapParser tileMapParser(path);
    tileMapParser.parse();
    // tileMapParser.printLayers();

    const TileMapData& tileMapData = tileMapParser.getTileMapData();

    // // print mTileInfoMap
    // for (const auto& pair : tileMapData.mTileInfoMap) {
    //     int gID = pair.first;
    //     const TileInfo& info = pair.second;
    //     std::cout << "gID: " << gID << ", Tileset: " << info.mTilesetName << ", Coordinates: ("
    //               << info.mCoordinates.first << ", " << info.mCoordinates.second << ")" << std::endl;
    // }

    // Assuming tileMapData is a const reference to TileMapData
    for (size_t layerIndex = 0; layerIndex < tileMapData.mLayers.size(); ++layerIndex) {
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
                            boxCollider->setTransformFromColliderData(collider);
                            gameObject->addComponent(boxCollider);
                        }

                        if (!tileInfo.mColliders.empty()) {
                            RigidBody* rigidBody = new RigidBody();
                            rigidBody->setTransform(objectTransform);
                            gameObject->addComponent(rigidBody);
                            gameObject->setName("Tile");
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

void InitBehaviourScript::onCollide(GameObject* aGameObject) {}
