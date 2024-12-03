#pragma once

#include "EnemyTransformSerialize.h"
#include "GameObject.h"
#include "Network/INetworkBehaviour.h"
#include "Network/NetworkVariable.h"
#include <iostream>
#include "Pathfinding.h"

class EnemyBehaviourScript : public INetworkBehaviour
{
public:
    EnemyBehaviourScript(std::shared_ptr<Pathfinding> pathfinding, int mapWidth) : mPathfinding(pathfinding), mMapWidth(mapWidth) {}
    void onStart() override;
    void onUpdate() override;
    void onCollide(GameObject* aGameObject) override;

    std::unique_ptr<Component> clone() const override { return std::make_unique<EnemyBehaviourScript>(*this); }
private: 
    void toggleAnimaionEnabled();
    void setAnimationActive(std::string aAnimationTag, bool aState);
    void deactivateAllAnimations();
    void setFlipX(bool aState);
    void setFlipY(bool aState);
    void findPathToPlayer();
    void move();

private:
    std::shared_ptr<Pathfinding> mPathfinding;
    int mMapWidth;

    std::vector<int> mPath;
    int mPathIndex = 0;
	NetworkVariable<EnemyTransformSerialize> mTransform{this};
};