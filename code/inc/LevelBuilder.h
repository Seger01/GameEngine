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
 */


class LevelBuilder {
public:
    LevelBuilder() = default;
    ~LevelBuilder() = default;

    void createLevel(Scene* scene, const TileMapData& tileMapData);

private:
    void createRoomEntry(Scene* scene, const MapObject& mapObject, const TileMapData& tileMapData) const;
    void createLevelEndTrigger(Scene* scene, const MapObject& mapObject) const;
    void createTileLayers(Scene* scene, const TileMapData& tileMapData) const;
    void createTile(Scene* scene, const TileInfo& tileInfo, int layerIndex, int rowIndex, int colIndex, bool isDoorsLayer, bool isGraphLayer) const;
    void addTriggerCollider(GameObject* gameObject, const MapObject& mapObject) const;
};