#include "SpriteDefUtil.h"

#include <vector>

#include "SpriteDef.h"

std::vector<SpriteDef> SpriteDefUtil::extrapolateSpriteDef(const SpriteDef& spriteDef, int numFrames) {
    std::vector<SpriteDef> spriteDefs;
    for (int i = 0; i < numFrames; i++) {
        SpriteDef newSpriteDef = spriteDef;

        newSpriteDef.sourceRect.x += i * spriteDef.sourceRect.w;

        spriteDefs.push_back(newSpriteDef);
    }
    return spriteDefs;
}
