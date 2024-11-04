#include "Window.h"
#include "SDL_video.h"
#include <stdexcept>

// Constructor
Window::Window(int aWindowWidth, int aWindowHeight) {
    // Initialize SDL if not already done
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw std::runtime_error("SDL could not initialize! SDL_Error: " + std::string(SDL_GetError()));
    }

    // Create the window
    mWindow = SDL_CreateWindow("SDL Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, aWindowWidth,
                               aWindowHeight, SDL_WINDOW_SHOWN);

    if (mWindow == nullptr) {
        throw std::runtime_error("Window could not be created! SDL_Error: " + std::string(SDL_GetError()));
    }
}

// Destructor
Window::~Window() {
    // Destroy window
    if (mWindow != nullptr) {
        SDL_DestroyWindow(mWindow);
    }
    // Quit SDL subsystems
    SDL_Quit();
}

// Get the window size as a Vector2 object
Vector2 Window::getSize() {
    int width, height;
    SDL_GetWindowSize(mWindow, &width, &height);
    return Vector2(width, height);
}

// Set the window to fullscreen mode
void Window::setFullScreen() {
    if (SDL_SetWindowFullscreen(mWindow, SDL_WINDOW_FULLSCREEN) != 0) {
        throw std::runtime_error("Unable to set fullscreen mode! SDL_Error: " + std::string(SDL_GetError()));
    }
}

// Set the window to floating mode (borderless window)
void Window::setFloating() { SDL_SetWindowBordered(mWindow, SDL_FALSE); }

// Toggle between fullscreen and floating mode
void Window::toggleFullFloating() {
    Uint32 windowFlags = SDL_GetWindowFlags(mWindow);

    if (windowFlags & SDL_WINDOW_FULLSCREEN) {
        // If it's currently fullscreen, set it to floating (borderless)
        setFloating();
    } else {
        // Otherwise, set it to fullscreen
        setFullScreen();
    }
}

// Get the raw SDL_Window pointer
SDL_Window* Window::getSDLWindow() { return mWindow; }
