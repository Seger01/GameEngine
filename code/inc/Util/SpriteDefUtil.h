/**
 * @file SpriteDefUtil.h
 * @brief Defines the SpriteDefUtil class, responsible for extrapolating sprite definitions.
 */

#ifndef SPRITEDEFUTIL_H
#define SPRITEDEFUTIL_H

#include <vector>

#include "Structs/SpriteDef.h"

/**
 * @class SpriteDefUtil
 * @brief Provides utility functions for sprite definitions.
 */
class SpriteDefUtil
{
public:
	static std::vector<SpriteDef> extrapolateSpriteDef(const SpriteDef& spriteDef, int numFrames);
};

#endif // SPRITEDEFUTIL_H
