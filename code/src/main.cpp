#include <string>
#include <vector>

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
#include <SDL_mixer.h>
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
    // if (SDL_Init(SDL_INIT_AUDIO) < 0) {
    //     printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    //     return -1;
    // }

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
    std::string soundPath = FSConverter().getResourcePath("gun1.wav");
    std::string musicPath = FSConverter().getResourcePath("music.wav");
    loadMusic(soundPath.c_str()); // Sound and music are swapped for
                                  // the demo; hearing the music
                                  // rotate is easier to distinguish
    loadSound(musicPath.c_str());

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

    Scene* scene = sceneManager.createScene("initscene");
    if (scene == nullptr)
        exit(1);

    int cameraID = scene->addCamera();
    scene->setActiveCamera(cameraID);

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
    // engineTest();

    sdlMixer();
}