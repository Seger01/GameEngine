#ifndef SPRITEDEF_H
#define SPRITEDEF_H

#include <string>

#include "Rect.h"

struct SpriteDef {
    std::string texturePath = "";
    Rect sourceRect; // is set to 0,0,0,0 on creation
    int width = 0, height = 0;
};

#endif
