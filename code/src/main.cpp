
#include <SDL.h>
#include <iostream>

#include "EngineBravo.h"

// // Initialize SDL and create a window and renderer
// bool init(SDL_Window*& window, SDL_Renderer*& renderer) {
//     if (SDL_Init(SDL_INIT_VIDEO) < 0) {
//         std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
//         return false;
//     }
//
//     // Create window
//     window = SDL_CreateWindow("SDL Rectangle Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600,
//                               SDL_WINDOW_SHOWN);
//     if (window == nullptr) {
//         std::cerr << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
//         return false;
//     }
//
//     // Create renderer for window
//     renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
//     if (renderer == nullptr) {
//         std::cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
//         return false;
//     }
//
//     return true;
// }

// int main(int argc, char* args[]) {
//     // SDL_Window* window = nullptr;
//     // SDL_Renderer* renderer = nullptr;
//
//     // // Initialize SDL and create window/renderer
//     // if (!init(window, renderer)) {
//     //     std::cerr << "Failed to initialize SDL!" << std::endl;
//     //     return -1;
//     // }
//     EngineBravo engine;
//
//     engine.getSceneManager().createScene("testje baby");
//     engine.getSceneManager().loadScene(0);
//
//     SDL_Window* window = engine.getRenderSystem().getWindow().getSDLWindow();
//     SDL_Renderer* renderer = engine.getRenderSystem().getRenderer().getSDLRenderer();
//
//     bool quit = false;
//     SDL_Event e;
//
//     engine.run();
//
//     // Cleanup
//     // SDL_DestroyRenderer(renderer);
//     // SDL_DestroyWindow(window);
//     // SDL_Quit();
//
//     return 0;
// }

#include <string>

#include "Animation.h"
#include "Components/Sprite.h"
#include "Engine/EngineBravo.h"
#include "Engine/SceneManager.h"
#include "FSConverter.h"
#include "Rectangle.h"
#include "Renderer.h"
#include "SampleBevahiourScript.h"
#include "Scene.h"
#include "SpriteAtlas.h"
#include "Texture.h"
#include "Window.h"
#include "test.h"

void workingPrototype() {
    EngineBravo engine;
    engine.initizalize();
    SceneManager& sceneManager = engine.getSceneManager();

    Scene* scene = sceneManager.createScene("Level1");
    if (scene == nullptr)
        exit(1);

    GameObject* gameObject = new GameObject;

    Transform objectTransform;

    objectTransform.position.x = 400;
    objectTransform.position.y = 400;

    gameObject->setTransform(objectTransform);

    scene->addGameObject(gameObject);

    sceneManager.loadScene(0);

    std::string spritePath = "player.png";

    // // Load the image from the specified path
    // SDL_Texture* loadedTexture = IMG_LoadTexture(engine.getRenderSystem().getRenderer().getSDLRenderer(),
    //                                              FSConverter().getResourcePath(spritePath).c_str());
    // if (loadedTexture == nullptr) {
    //     std::cerr << "Unable to load texture! SDL_image Error: " << IMG_GetError() << std::endl;
    // }

    // Texture* texture = new Texture(loadedTexture);
    Texture* texture = engine.getResourceManager().loadTexture(spritePath);

    Sprite* sprite = new Sprite(texture);

    Transform newTransform;

    newTransform.position.x = 0;
    newTransform.position.y = 0;

    sprite->setWidth(200);
    sprite->setHeight(300);

    sprite->setTransform(newTransform);

    gameObject->addComponent(sprite);

    engine.run();

    return;
}

int main() {
    workingPrototype();

    return 0;
}
