#ifndef SPRITEDEFUTIL_H
#define SPRITEDEFUTIL_H

#include <vector>

#include "SpriteDef.h"

class SpriteDefUtil {
public:
    static std::vector<SpriteDef> extrapolateSpriteDef(const SpriteDef& spriteDef, int numFrames);
};

#endif // SPRITEDEFUTIL_H
