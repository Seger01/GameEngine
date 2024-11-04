#include <string>
#include <vector>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_render.h>
#include <SDL_video.h>

#include "Animation.h"
#include "EngineBravo.h"
#include "FSConverter.h"
#include "PlayerBehaviourScript.h"
#include "Renderer.h"
#include "SampleBevahiourScript.h"
#include "SaveGameManager.hpp"
#include "Scene.h"
#include "SceneManager.h"
#include "Sprite.h"
#include "SpriteAtlas.h"
#include "SpriteDef.h"
#include "Texture.h"
#include "Window.h"
#include "test.h"

void engineTest() {
    EngineBravo& engine = EngineBravo::getInstance();
    SceneManager& sceneManager = engine.getSceneManager();
    SaveGameManager& savegameManager = engine.getSaveGameManager();

    Scene* scene = sceneManager.createScene("Level1");
    if (scene == nullptr)
        exit(1);

    GameObject* gameObject = new GameObject;

    Transform objectTransform;

    objectTransform.position.x = 400;
    objectTransform.position.y = 400;

    gameObject->setTransform(objectTransform);

    gameObject->addComponent<PlayerBehaviourScript>();

    scene->addGameObject(gameObject);

    sceneManager.loadScene(0);

    engine.initizalize();
    engine.run();

    return;
}

int main() {
    engineTest();
    // // Initialize SDL
    // if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    //     printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    //     return 1;
    // }

    // // Create a window
    // SDL_Window* window = SDL_CreateWindow("SDL Window",
    //     SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
    //     640, 480, SDL_WINDOW_SHOWN);

    // if (window == NULL) {
    //     printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    //     SDL_Quit();
    //     return 1;
    // }

    // // Main loop flag
    // int quit = 0;
    // SDL_Event event;

    // // Event loop
    // while (!quit) {
    //     // Handle events
    //     while (SDL_PollEvent(&event) != 0) {
    //         if (event.type == SDL_QUIT) {
    //             quit = 1;  // Set the quit flag to true
    //         }
    //     }

    //     // Here you can add rendering code if needed
    // }

    // // Clean up and close the window
    // SDL_DestroyWindow(window);
    // SDL_Quit();

    // return 0;
}
