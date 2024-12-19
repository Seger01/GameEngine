/**
 * @file LevelBuilder.h
 * @brief Contains the definition of the LevelBuilder class.
 */

#pragma once

#include "GameObject.h"
#include "Component.h"
#include "Transform.h"
#include <algorithm>
#include <iostream>
#include "Scene.h"
#include "TileMapParser.h"

/**
 * @class LevelBuilder
 *
 * @brief Builds a level from parsed json data.
 * @details This class creates a level from the parsed json data by creating game objects with sprites, colliders and rigid bodies.
 */


class LevelBuilder {
public:
    LevelBuilder() = default;
    ~LevelBuilder() = default;

    void createLevel(Scene& aScene, const TileMapData& aTileMapData, int aTileWidth, int aTileHeight);

private:
    void createTileLayers(Scene& aScene, const TileMapData& aTileMapData, int aTileWidth, int aTileHeight) const;
    void createTile(Scene& aScene, const TileInfo& aTileInfo, const std::string& aLayerName, int aLayerIndex, int aRowIndex, int aColIndex, int aTileWidth, int aTileHeight) const;
};