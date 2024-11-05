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
#include "SaveGameManager.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Sprite.h"
#include "SpriteAtlas.h"
#include "SpriteDef.h"
#include "Texture.h"
#include "TileMapParser.h"
#include "Transform.h"
#include "Window.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <chrono>
#include <iostream>
#include <thread>

std::vector<Mix_Chunk*> sounds;
std::vector<Mix_Music*> music;

int loadMusic(const char* filename) {
    Mix_Music* m = NULL;
    m = Mix_LoadMUS(filename);
    if (m == NULL) {
        printf("Failed to load music. SDL_Mixer error: %s\n", Mix_GetError());
        return -1;
    }
    music.push_back(m);
    return music.size() - 1;
}

int loadSound(const char* filename) {
    Mix_Chunk* m = NULL;
    m = Mix_LoadWAV(filename);
    if (m == NULL) {
        printf("Failed to load sound. SDL_Mixer error: %s\n", Mix_GetError());
        return -1;
    }
    sounds.push_back(m);
    return sounds.size() - 1;
}

int init() {
    // Initialize SDL with audio support
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    // Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        return -1;
    }

    return 0;
}

int playMusic(int m) {
    if (Mix_PlayingMusic() == 0) {
        Mix_VolumeMusic(50);

        Mix_PlayMusic(music[m],
                      -1); // Play the music (-1 means loop infinitely, 0 means play once)
    }
    return 0;
}

int playSound(int s) {
    Mix_Volume(0, 100);
    // Channel number, sound object and loop infinitely
    Mix_PlayChannel(1, sounds[s], 1);
    return 0;
}

void finalize() {
    for (int s = 0; s < sounds.size(); s++) {
        Mix_FreeChunk(sounds[s]);
        sounds[s] = NULL;
    }
    for (int s = 0; s < music.size(); s++) {
        Mix_FreeMusic(music[s]);
        music[s] = NULL;
    }
    Mix_Quit();
}

void sdlMixer() {
    init();
    loadMusic("../../../resources/gun1.wav"); // Sound and music are swapped for
                                              // the demo; hearing the music
                                              // rotate is easier to distinguish
    loadSound("../../../resources/music.wav");

    playMusic(0); // Play music at index 0
    int angle = 0;

    playSound(0); // Play sound at index 0

    // Wait until the music has finished playing
    while (Mix_Playing(1)) {
        // while (Mix_PlayingMusic()) {
        angle %= 360;
        SDL_Delay(100); // Sleep for 100 ms
        Mix_SetPosition(1, angle, 100);
        angle += 10;
        std::cout << angle << std::endl;
    }

    // Clean up
    finalize();
}

void engineTest() {
    EngineBravo& engine = EngineBravo::getInstance();
    SceneManager& sceneManager = engine.getSceneManager();
    SaveGameManager& savegameManager = engine.getSaveGameManager();
    SaveGame& sg1 = savegameManager.createSaveGame("save1", "saves/save1.save");

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

    sg1.setIntField("x", gameObject->getTransform().position.x);
    sg1.setIntField("y", gameObject->getTransform().position.y);
    sg1.store();

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