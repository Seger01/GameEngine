#include "SpriteDefUtil.h"

#include <vector>

#include "SpriteDef.h"

/**
 * @brief Extrapolates a sprite definition.
 * @param spriteDef The sprite definition to extrapolate.
 * @param numFrames The number of frames to extrapolate.
 * @return A vector of sprite definitions.
 */
std::vector<SpriteDef> SpriteDefUtil::extrapolateSpriteDef(const SpriteDef& spriteDef, int numFrames)
{
	std::vector<SpriteDef> spriteDefs;
	for (int i = 0; i < numFrames; i++)
	{
		SpriteDef newSpriteDef = spriteDef;

		newSpriteDef.sourceRect.x += i * spriteDef.sourceRect.w;

		spriteDefs.push_back(newSpriteDef);
	}
	return spriteDefs;
}
