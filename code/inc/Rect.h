#ifndef RECT_H
#define RECT_H

#include <SDL.h>

struct Rect {
    int x = 0, y = 0;
    int w = 0, h = 0;
    explicit operator SDL_Rect() { return {x, y, w, h}; }
};

#endif
