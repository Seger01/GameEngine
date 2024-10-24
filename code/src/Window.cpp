#include "Window.h"

Window::Window() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return;
    }

    // Create window
    mWindow = SDL_CreateWindow("SDL2 Sprite Sheet", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480,
                               SDL_WINDOW_SHOWN);
    if (mWindow == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return;
    }
}

Window::~Window() {
    SDL_DestroyWindow(this->mWindow);
    SDL_Quit();
}

SDL_Window*& Window::getSDLWindow() { return mWindow; }
