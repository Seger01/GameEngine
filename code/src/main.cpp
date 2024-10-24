#include <string>

#include "Components/Sprite.h"
#include "Engine/EngineBravo.h"
#include "Engine/SceneManager.h"
#include "SampleBevahiourScript.h"
#include "Scene.h"
#include "test.h"

int main() {
    EngineBravo engine;
    engine.initizalize();
    SceneManager& sceneManager = engine.getSceneManager();

    Scene* scene = sceneManager.createScene("Level1");
    if (scene == nullptr)
        exit(1);

    GameObject* gameObject = new GameObject;

    gameObject->addComponent<Sprite>();
    gameObject->addComponent<SampleBehaviourScript>();
    gameObject->getComponent<Sprite>()->setFlipX(true);

    scene->addGameObject(gameObject);

    sceneManager.loadScene(0);

    // std::cout << "flipX: " << gameObject->getComponent<Sprite>()->getFlipX() << std::endl;

    engine.run();

    return 0;
}

// #include <SDL2/SDL.h>
// #include <SDL2/SDL_image.h>
// #include <SDL2/SDL_render.h>
// #include <SDL2/SDL_video.h>
//
// #include "Animation.h"
// #include "SpriteAtlas.h"
//
// const int MOVE_SPEED = 10;
//
// int initSDL(SDL_Window*& window, SDL_Renderer*& renderer) {
//     // Initialize SDL
//     if (SDL_Init(SDL_INIT_VIDEO) < 0) {
//         printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
//         return -1;
//     }
//
//     // Create window
//     window = SDL_CreateWindow("SDL2 Sprite Sheet", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480,
//                               SDL_WINDOW_SHOWN);
//     if (window == NULL) {
//         printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
//         return -1;
//     }
//
//     // Create renderer
//     renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
//     if (renderer == NULL) {
//         printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
//         return -1;
//     }
//
//     return 0;
// }
//
// void deInitSDL(SDL_Window*& window, SDL_Renderer*& renderer) {
//     SDL_DestroyRenderer(renderer);
//     SDL_DestroyWindow(window);
//     SDL_Quit();
// }
//
// int main(int argc, char* args[]) {
//     SDL_Window* window = nullptr;
//     SDL_Renderer* renderer = nullptr;
//
//     initSDL(window, renderer);
//
//     bool quit = false;
//     SDL_Event event;
//
//     // Variables for animation timing and sprite movement
//     Uint32 lastTime = 0;         // Time of the last frame change
//     int frame = 0;               // Current frame (sprite in the sprite sheet)
//     const int spriteWidth = 16;  // Width of each sprite
//     const int spriteHeight = 25; // Height of each sprite
//     const int frameCount = 6;    // Total number of frames in the sprite sheet
//
//     // SDL_Texture* spriteSheetTexture = loadTexture(renderer, "enter_the_gungeon_spritesheet.png");
//     SpriteAtlas spriteAtlas(renderer, "enter_the_gungeon_spritesheet.png");
//
//     SDL_Rect startOfAnimation;
//     startOfAnimation.x = 22;          // Move horizontally in the sprite sheet
//     startOfAnimation.y = 187;         // Keep the vertical position constant (you can change this for vertical
//     movement) startOfAnimation.w = spriteWidth; // The width of the sprite startOfAnimation.h = spriteHeight; // The
//     height of the sprite
//
//     Animation& animation = spriteAtlas.getAnimation(startOfAnimation, frameCount);
//
//     // Define the destination rect where the image will be drawn
//     SDL_Rect destRect;
//     destRect.x = 100;    // The x position on the screen
//     destRect.y = 100;    // The y position on the screen
//     destRect.w = 18 * 4; // The width of the drawn image (scaling)
//     destRect.h = 26 * 4; // The height of the drawn image (scaling)
//
//     while (!quit) {
//         // Event handling
//         while (SDL_PollEvent(&event) != 0) {
//             if (event.type == SDL_QUIT) {
//                 quit = true;
//             }
//             if (event.type == SDL_KEYDOWN) {
//                 switch (event.key.keysym.sym) {
//                 case SDLK_w: // Move up
//                     destRect.y -= MOVE_SPEED;
//                     break;
//                 case SDLK_s: // Move down
//                     destRect.y += MOVE_SPEED;
//                     break;
//                 case SDLK_a: // Move left
//                     destRect.x -= MOVE_SPEED;
//                     break;
//                 case SDLK_d: // Move right
//                     destRect.x += MOVE_SPEED;
//                     break;
//                 }
//             }
//         }
//         // Clear screen
//         SDL_RenderClear(renderer);
//
//         SDL_Rect srcRect = animation.getCurrentFrame();
//         SDL_Texture* spriteTexture = animation.getTexture();
//
//         // Copy part of the sprite sheet to the renderer
//         SDL_RenderCopy(renderer, spriteTexture, &srcRect, &destRect);
//
//         // Update the screen
//         SDL_RenderPresent(renderer);
//     }
//
//     // Clean up
//     deInitSDL(window, renderer);
//     return 0;
// >>>>>>> origin/poc_animation
// }
