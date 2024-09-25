#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "SDL.h"

struct Rectangle {
    int x, y;
    int w, h;
    explicit operator SDL_Rect() { return {x, y, w, h}; }
};

#endif
