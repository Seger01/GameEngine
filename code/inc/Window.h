#ifndef WINDOW_H
#define WINDOW_H

#include <SDL.h>

#include "Point.h"

class Window {
public:
    Window(int aWindowWidth, int aWindowHeight);
    ~Window();

    Point getSize();

    void setFullScreen();
    void setFloating();
    void toggleFullFloating();

    SDL_Window* getSDLWindow();

private:
    SDL_Window* mWindow = nullptr;
};

#endif
