#pragma once

#include "IBehaviourScript.h"
#include "GameObject.h"
#include <iostream>
#include <vector>
#include "TileMapParser.h"
#include "SpriteDef.h"

class RoomBehaviourScript : public IBehaviourScript {
public:
    RoomBehaviourScript(const std::string& roomID, const std::vector<SpawnPoint>& enemySpawns)
        : mRoomID(roomID), mEnemySpawns(enemySpawns) {}
    void onStart() override;
    void onUpdate() override;
    void onCollide(GameObject* aGameObject) override;

    void onTriggerEnter(GameObject* aPlayer);
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
    bool mDoorsOpen = true;
};