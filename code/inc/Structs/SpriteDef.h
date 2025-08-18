/**
 * @file SpriteDef.h
 * @brief Defines the SpriteDef struct.
 */

#ifndef SPRITEDEF_H
#define SPRITEDEF_H

#include <string>

#include "Structs/Rect.h"

/**
 * @struct SpriteDef
 * @brief Defines a sprite definition.
 */
struct SpriteDef
{
	std::string texturePath = "";
	Rect sourceRect; // is set to 0,0,0,0 on creation
	float width = 0, height = 0;
};

#endif
