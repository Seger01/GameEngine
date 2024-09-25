#ifndef WINDOW_H
#define WINDOW_H

#include <SDL.h>

class Window {
public:
    Window();
    ~Window();

    SDL_Window*& getSDLWindow();

private:
    SDL_Window* mWindow = nullptr;
};

#endif
