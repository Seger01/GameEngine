
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

int main(int argc, char* args[]) {
    // SDL_Window* window = nullptr;
    // SDL_Renderer* renderer = nullptr;

    // // Initialize SDL and create window/renderer
    // if (!init(window, renderer)) {
    //     std::cerr << "Failed to initialize SDL!" << std::endl;
    //     return -1;
    // }
    EngineBravo engine;

    SDL_Window* window = engine.getRenderSystem().getWindow().getSDLWindow();
    SDL_Renderer* renderer = engine.getRenderSystem().getRenderer().getSDLRenderer();

    bool quit = false;
    SDL_Event e;

    // Main loop
    while (!quit) {
        // Event handling
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Red
        // // Clear the screen
        // SDL_RenderClear(renderer);
        engine.getRenderSystem().getRenderer().clear(Color(0, 0, 0));

        // Set the draw color for the renderer (red, green, blue, alpha)
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red

        // Create a rectangle at position (200, 150) with width 400 and height 300
        SDL_Rect rect = {200, 150, 400, 300};

        // Draw an empty rectangle (outline)
        SDL_RenderDrawRect(renderer, &rect);

        // Set a new color (green) and fill the rectangle
        // SDL_RenderFillRect(renderer, &rect);

        // // Update the screen with the drawn rectangle
        // SDL_RenderPresent(renderer);
        engine.getRenderSystem().getRenderer().show();
    }

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

// #include <string>
//
// #include "Animation.h"
// #include "Components/Sprite.h"
// #include "Engine/EngineBravo.h"
// #include "Engine/SceneManager.h"
// #include "FSConverter.h"
// #include "Rectangle.h"
// #include "Renderer.h"
// #include "SampleBevahiourScript.h"
// #include "Scene.h"
// #include "SpriteAtlas.h"
// #include "Texture.h"
// #include "Window.h"
// #include "test.h"
//
// int main() {
//     EngineBravo engine;
//     engine.initizalize();
//     SceneManager& sceneManager = engine.getSceneManager();
//
//     Scene* scene = sceneManager.createScene("Level1");
//     if (scene == nullptr)
//         exit(1);
//
//     GameObject* gameObject = new GameObject;
//
//     // gameObject->addComponent<Sprite>();
//     // gameObject->addComponent<SampleBehaviourScript>();
//     // gameObject->getComponent<Sprite>()->setFlipX(true);
//
//     scene->addGameObject(gameObject);
//
//     sceneManager.loadScene(0);
//
//     std::string spritePath = "player.png";
//
//     // Initialize PNG loading
//     int imgFlags = IMG_INIT_PNG;
//     if (!(IMG_Init(imgFlags) & imgFlags)) {
//         std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
//         return 0;
//     }
//
//     // // Load image at specified path
//     // SDL_Surface* loadedSurface = IMG_Load(FSConverter().getResourcePath(spritePath).c_str());
//     // if (loadedSurface == nullptr) {
//     //     throw std::runtime_error(
//     //         "Unable to load image " + spritePath + "! SDL_image Error: " + std::string(IMG_GetError()));
//     // }
//
//     // // Load the image from the specified path
//     // SDL_Texture* mTexture = IMG_LoadTexture(engine.getRenderSystem().getRenderer().getSDLRenderer(),
//     //                                         FSConverter().getResourcePath(spritePath).c_str());
//     // if (mTexture == nullptr) {
//     //     std::cerr << "Unable to load texture! SDL_image Error: " << IMG_GetError() << std::endl;
//     // }
//
//     // // Create texture from surface pixels
//     // SDL_Texture* mTexture =
//     //     SDL_CreateTextureFromSurface(engine.getRenderSystem().getRenderer().getSDLRenderer(), loadedSurface);
//     // if (mTexture == nullptr) {
//     //     SDL_FreeSurface(loadedSurface);
//     //     throw std::runtime_error(
//     //         "Unable to create texture from " + spritePath + "! SDL Error: " + std::string(SDL_GetError()));
//     // }
//
//     // Free the loaded surface as we no longer need it
//     // SDL_FreeSurface(loadedSurface);
//     //
//     SDL_Texture* emptyTexture = nullptr;
//
//     Texture texture(emptyTexture);
//
//     Sprite* sprite = new Sprite(&texture);
//
//     // sprite->getTransformPtr()->position.x = 500;
//     // sprite->getTransformPtr()->position.y = 500;
//     //
//     // sprite->setWidth(40);
//     // sprite->setHeight(40);
//
//     gameObject->addComponent(sprite);
//
//     // std::cout << "flipX: " << gameObject->getComponent<Sprite>()->getFlipX() << std::endl;
//
//     while (true) {
//         // Clear the screen
//         SDL_SetRenderDrawColor(engine.getRenderSystem().getRenderer().getSDLRenderer(), 0, 0, 0, 255); // Green
//         SDL_RenderClear(engine.getRenderSystem().getRenderer().getSDLRenderer());
//
//         // Set the draw color for the renderer (red, green, blue, alpha)
//         SDL_SetRenderDrawColor(engine.getRenderSystem().getRenderer().getSDLRenderer(), 255, 0, 0, 255); // Red
//
//         // Create a rectangle at position (200, 150) with width 400 and height 300
//         SDL_Rect rect = {200, 150, 400, 300};
//
//         // Draw an empty rectangle (outline)
//         SDL_RenderDrawRect(engine.getRenderSystem().getRenderer().getSDLRenderer(), &rect);
//
//         // Set a new color (green) and fill the rectangle
//         SDL_SetRenderDrawColor(engine.getRenderSystem().getRenderer().getSDLRenderer(), 0, 255, 0, 255); // Green
//         SDL_RenderFillRect(engine.getRenderSystem().getRenderer().getSDLRenderer(), &rect);
//
//         // Update the screen with the drawn rectangle
//         SDL_RenderPresent(engine.getRenderSystem().getRenderer().getSDLRenderer());
//
//         SDL_Delay(16);
//     }
//
//     engine.run();
//
//     return 0;
// }
