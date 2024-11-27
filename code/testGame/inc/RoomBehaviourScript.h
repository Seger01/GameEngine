#pragma once

#include "IBehaviourScript.h"
#include "GameObject.h"
#include <iostream>
#include <vector>
#include "TileMapParser.h"
#include "SpriteDef.h"
#include "Pathfinding.h"


class RoomBehaviourScript : public IBehaviourScript {
public:
    RoomBehaviourScript(const std::string& roomID, const std::vector<SpawnPoint>& enemySpawns, std::shared_ptr<Pathfinding> pathfinding, int mapWidth)
        : mRoomID(roomID), mEnemySpawns(enemySpawns), mPathfinding(pathfinding), mMapWidth(mapWidth) {}

    void onStart() override;
    void onUpdate() override;
    void onCollide(GameObject* aGameObject) override;
    std::unique_ptr<Component> clone() const override { return std::make_unique<RoomBehaviourScript>(*this); }

private:
    void spawnEnemies();
    void updateDoors(const SpriteDef& spriteDef);
    void openDoors();
    void closeDoors();

private:
    std::string mRoomID;
    std::vector<SpawnPoint> mEnemySpawns;
    SpriteDef mClosedDoorSpriteDef;
    SpriteDef mOpenDoorSpriteDef;
    SpriteDef mEnemyFrameDef;
    std::shared_ptr<Pathfinding> mPathfinding;
    int mMapWidth;
    bool mDoorsOpen = true;
};
