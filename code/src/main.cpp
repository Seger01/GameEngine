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
    // EventManager eventManager;
    // eventManager.subscribe(onMouseDownEvent, EventType::MouseButtonDown);
    // eventManager.subscribe(enditall, EventType::Quit);
    // eventManager.subscribe(handlePlayerMovement, EventType::DefinedAction);
    // eventManager.subscribe(keyDown, EventType::KeyDown);

    // eventManager.subscribe(anyEvent);

    Input& input = Input::getInstance();

    input.setActiveContext("Playing");

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
        input.update();

        if (input.GetKey(Key::Key_Q)) {
            quit = true;
        }
        if (input.GetKeyDown(Key::Key_T)) {
            input.setActiveContext("InMenu");
        }
        if (input.GetKeyDown(Key::Key_G)) {
            input.setActiveContext("Playing");
        }

        if (input.GetAction(DefAction::Move_Up)) {
            destRect.y -= MOVE_SPEED;
        }
        if (input.GetAction(DefAction::Move_Left)) {
            destRect.x -= MOVE_SPEED;
        }
        if (input.GetAction(DefAction::Move_Down)) {
            destRect.y += MOVE_SPEED;
        }
        if (input.GetAction(DefAction::Move_Right)) {
            destRect.x += MOVE_SPEED;
        }

        // Process events
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Clear screen
        myRenderer->clear();

        // Copy part of the sprite sheet to the renderer
        myRenderer->renderAnimation(animation, destRect);

        // Update the screen
        myRenderer->present();

        SDL_Delay(16);
    }
    return;
}

void testControllerReading() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    // Check for any available game controllers
    if (int amountOfJoysticks = SDL_NumJoysticks() < 1) {
        std::cout << "No game controllers connected!" << std::endl;
    } else {
        // Open the first available controller
        SDL_GameController* controller = SDL_GameControllerOpen(0);
        if (controller == nullptr) {
            std::cerr << "Could not open game controller! SDL_Error: " << SDL_GetError() << std::endl;
        } else {
            bool quit = false;

            // Game loop
            while (!quit) {
                std::cout << "Amount of Joysticks connected: " << amountOfJoysticks << std::endl;

                // Handle quitting (e.g., from window close events or keyboard input)
                SDL_Event event;
                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT) {
                        quit = true;
                    }
                }

                // Read the controller button states
                if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A)) {
                    std::cout << "A button is pressed!" << std::endl;
                }
                if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_B)) {
                    std::cout << "B button is pressed!" << std::endl;
                }

                // Read the controller axis states (e.g., left joystick)
                int leftX = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX);
                int leftY = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY);
                std::cout << "Left joystick X: " << leftX << " Y: " << leftY << std::endl;

                // Simple delay to limit CPU usage (for demonstration purposes)
                SDL_Delay(100);
            }

            // Close the controller
            SDL_GameControllerClose(controller);
        }
    }
    // Quit SDL subsystems
    SDL_Quit();
}
int main() {
    run();

    return 0;
}
