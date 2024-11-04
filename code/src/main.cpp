#include <string>
#include <vector>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_render.h>
#include <SDL_video.h>

#include "Animation.h"
#include "EngineBravo.h"
#include "FSConverter.h"
#include "GameObject.h"
#include "InitBehaviourScript.h"
#include "PlayerBehaviourScript.h"
#include "Renderer.h"
#include "SampleBevahiourScript.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Sprite.h"
#include "SpriteAtlas.h"
#include "SpriteDef.h"
#include "Texture.h"
#include "TileMapParser.h"
#include "Transform.h"
#include "Window.h"
#include "test.h"

void engineTest() {
    EngineBravo& engine = EngineBravo::getInstance();
    SceneManager& sceneManager = engine.getSceneManager();

    Scene* scene = sceneManager.createScene("initscene");
    if (scene == nullptr)
        exit(1);

    int cameraID = scene->addCamera();
    scene->setActiveGamera(cameraID);

    scene->getActiveCamera().setTransform(Transform(Vector2(80, 0)));
    scene->getActiveCamera().setWidth(160);
    scene->getActiveCamera().setHeight(90);

    GameObject* gameObject = new GameObject;

    gameObject->addComponent<InitBehaviourScript>();

    scene->addGameObject(gameObject);

    sceneManager.requestSceneChange("initscene");

    engine.initizalize();
    engine.run();

    return;
}

int main() {
    engineTest();
    return 0;
    FSConverter fsConverter;
    std::string path = fsConverter.getResourcePath("LevelDefs/level.json");

    TileMapParser tileMapParser(path);
    tileMapParser.parse();
    // tileMapParser.printLayers();

    // Print the contents of tileInfoMap
    const TileMapData& tileMapData = tileMapParser.getTileMapData();
    // print layers in tilemap
    for (size_t layerIndex = 0; layerIndex < tileMapData.mLayers.size(); ++layerIndex) {
        std::cout << "Layer " << layerIndex << ":\n";
        for (const auto& row : tileMapData.mLayers[layerIndex]) {
            for (int tile : row) {
                std::cout << tile << " ";
            }
            std::cout << "\n";
        }
    }

    for (const auto& pair : tileMapData.mTileInfoMap) {
        int gID = pair.first;
        const TileInfo& info = pair.second;
        std::cout << "gID: " << gID << ", Tileset: " << info.mTilesetName << ", Coordinates: ("
                  << info.mCoordinates.first << ", " << info.mCoordinates.second << ")" << std::endl;
    }

    return 0;

    // Struct ParsedScene
    // vector intergers to save layers
    // 2D array of grid per layer
    // Map to link gID to tileset
}
