#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_render.h>
#include <SDL_video.h>

#include "Animation.h"
#include "Rectangle.h"
#include "Renderer.h"
#include "SpriteAtlas.h"
#include "Window.h"

#include "Event.h"
#include "Input.h"

const int MOVE_SPEED = 10;

// class Input {
// public:
//     Input() {}
//
//     void subscribe(DefAction aDefAction, const std::function<void(float)>& callback) {
//         mContextManager.subscribeToAction(aDefAction, callback);
//     }
//
//     void update() {
//         int numKeys;
//         const Uint8* allKeys = SDL_GetKeyboardState(&numKeys);
//
//         for (int i = 0; i < numKeys; ++i) {
//             if (allKeys[i]) {
//                 // std::cout << "Key with scancode " << keyToString((Key)i) << " is pressed" << std::endl;
//                 mContextManager.processKey((Key)i);
//             }
//         }
//
//         return;
//     }
//
//     bool getKeyDown(Key aKeyToCheck) { return false; }
//     bool isKeyDown() { return false; }
//
// private:
//     ContextManager mContextManager;
//
//     Uint8* mAllKeys = nullptr;
// };

void onMouseDownEvent(const Event& aEvent) {
    if (aEvent.mouse.left) {
        std::cout << "mouse left pressed at " << aEvent.mouse.position.x << ", " << aEvent.mouse.position.y
                  << std::endl;
    }
    if (aEvent.mouse.middle) {
        std::cout << "mouse middle pressed at " << aEvent.mouse.position.x << ", " << aEvent.mouse.position.y
                  << std::endl;
    }
    if (aEvent.mouse.right) {
        std::cout << "mouse right pressed at " << aEvent.mouse.position.x << ", " << aEvent.mouse.position.y
                  << std::endl;
    }
}

bool quit = false;

void enditall(const Event& event) { quit = true; }

void anyEvent(const Event& aEvent) {
    std::cout << "this will trigger with every event!!!" << std::endl;

    return;
}

void handlePlayerMovement(const Event& aEvent) {
    std::cout << "Player movement detected" << std::endl;

    return;
}

void keyDown(const Event& aEvent) { std::cout << "Key Down Event" << std::endl; }

void run() {
    // contextManager.setActiveContext("Playing");
    EventManager eventManager;
    eventManager.subscribe(onMouseDownEvent, EventType::MouseButtonDown);
    eventManager.subscribe(enditall, EventType::Quit);
    eventManager.subscribe(handlePlayerMovement, EventType::DefinedAction);
    eventManager.subscribe(keyDown, EventType::KeyDown);

    // eventManager.subscribe(anyEvent);

    Input& input = Input::getInstance();

    Window myWindow;
    Renderer* myRenderer = new Renderer(myWindow);

    SDL_Event event;

    // Variables for animation timing and sprite movement
    Uint32 lastTime = 0;         // Time of the last frame change
    int frame = 0;               // Current frame (sprite in the sprite sheet)
    const int spriteWidth = 16;  // Width of each sprite
    const int spriteHeight = 25; // Height of each sprite
    const int frameCount = 6;    // Total number of frames in the sprite sheet

    // SDL_Texture* spriteSheetTexture = loadTexture(renderer, "enter_the_gungeon_spritesheet.png");
    SpriteAtlas spriteAtlas(myRenderer, "enter_the_gungeon_spritesheet.png");

    Rectangle startOfAnimation;
    startOfAnimation.x = 22;          // Move horizontally in the sprite sheet
    startOfAnimation.y = 187;         // Keep the vertical position constant (you can change this for vertical movement)
    startOfAnimation.w = spriteWidth; // The width of the sprite
    startOfAnimation.h = spriteHeight; // The height of the sprite

    Animation& animation = spriteAtlas.getAnimation(startOfAnimation, frameCount);

    Rectangle destRect;
    // Define the destination rect where the image will be drawn
    destRect.x = 100;    // The x position on the screen
    destRect.y = 100;    // The y position on the screen
    destRect.w = 18 * 4; // The width of the drawn image (scaling)
    destRect.h = 26 * 4; // The height of the drawn image (scaling)

    while (!quit) {
        // while (SDL_PollEvent(&event) != 0) {
        //     if (event.type == SDL_QUIT) {
        //         quit = true;
        //     }
        // }
        // input.update();

        // input.print();

        // if (input.GetKey(Key::Key_W)) {
        //     destRect.y -= MOVE_SPEED;
        // }
        // if (input.GetKey(Key::Key_A)) {
        //     destRect.x -= MOVE_SPEED;
        // }
        // if (input.GetKey(Key::Key_S)) {
        //     destRect.y += MOVE_SPEED;
        // }
        // if (input.GetKey(Key::Key_D)) {
        //     destRect.x += MOVE_SPEED;
        // }
        //
        // if (input.GetActionRange(DefAction::Move_Up)) {
        //     destRect.y -= MOVE_SPEED;
        // }
        //
        // if (input.GetMouseButtonDown(MouseButton::LEFT)) {
        //     std::cout << "Mouse button left pressed" << input.MousePosition().x << ", " << input.MousePosition().y
        //               << std::endl;
        // }
        input.update();

        eventManager.handleEvents();

        // Clear screen
        // SDL_RenderClear(renderer);
        myRenderer->clear();

        // Texture* spriteTexture = animation.getTexture();

        // Copy part of the sprite sheet to the renderer
        // SDL_RenderCopy(renderer, spriteTexture->getSDL_Texture(), &srcRect, &destRect);
        myRenderer->renderAnimation(animation, destRect);

        // Update the screen
        // SDL_RenderPresent(renderer);
        myRenderer->present();

        SDL_Delay(16);
    }
    // Clean up
    // deInitSDL(window, renderer);
    return;
}
int main() {
    run();

    // std::vector<Key> keys = {Key::Key_W, Key::Key_A, Key::Key_S};
    //
    // DefinedAction action(0, keys);
    //
    // action.print();

    // // Subscribe external functions
    // context.subscribe(externalFunction1);
    // context.subscribe(externalFunction2);
    //
    // // Subscribe a lambda function for variety
    // context.subscribe([](float value) { std::cout << "Lambda function received value: " << value << std::endl;
    // });
    //
    // // Simulate an update
    // context.update(42.0f);

    // for (int i = 0; i < action.mTriggerKeys.size(); i++) {
    //     std::cout << static_cast<int>(action.mTriggerKeys[i]) << std::endl;
    // }

    return 0;
}
