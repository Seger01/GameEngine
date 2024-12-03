#include "EnemyBehaviourScript.h"
#include "Color.h"
#include "EngineBravo.h"
#include "RenderSystem.h"
#include "SceneManager.h"

void EnemyBehaviourScript::onStart() { findPathToPlayer(); }

void EnemyBehaviourScript::onUpdate() { 
    move();
	if (isOwner())
	{ // if owner set the data to be synchronized with the network variable
		mTransform.getValue().setTransform(mGameObject->getTransform());
	}
	else
	{ // if not owner set the data to be synchronized with the network variable
		mGameObject->getTransform().position = mTransform.getValue().getTransform().position;
	}
}

void EnemyBehaviourScript::onCollide(GameObject* aGameObject) {}

void EnemyBehaviourScript::toggleAnimaionEnabled() {}

void EnemyBehaviourScript::setAnimationActive(std::string aAnimationTag, bool aState) {}

void EnemyBehaviourScript::deactivateAllAnimations() {}

void EnemyBehaviourScript::setFlipX(bool aState) {}

void EnemyBehaviourScript::setFlipY(bool aState) {}

void EnemyBehaviourScript::move() {
    if (mPathIndex < mPath.size()) {
        int targetNode = mPath[mPathIndex];
        int targetX = targetNode % mMapWidth;
        int targetY = targetNode / mMapWidth;

        Transform enemyTransform = mGameObject->getTransform();

        int currentTileX = static_cast<int>(enemyTransform.position.x / 16);
        int currentTileY = static_cast<int>(enemyTransform.position.y / 16);

        // Determine movement direction
        int moveX = (targetX > currentTileX) ? 1 : (targetX < currentTileX) ? -1 : 0;
        int moveY = (targetY > currentTileY) ? 1 : (targetY < currentTileY) ? -1 : 0;

        // Move one pixel at a time
        enemyTransform.position.x += moveX;
        enemyTransform.position.y += moveY;

        mGameObject->setTransform(enemyTransform);
        // Check if reached the target tile
        if (currentTileX == targetX && currentTileY == targetY) {
            mPathIndex++;
        }
    }
}

void EnemyBehaviourScript::findPathToPlayer() {
    EngineBravo& engine = EngineBravo::getInstance();
    SceneManager& sceneManager = engine.getSceneManager();
    Scene* scene = sceneManager.getCurrentScene();

    std::vector<GameObject*> playerObjs = scene->getGameObjectsWithTag("Player");
    if (playerObjs.empty()) {
        std::cerr << "Player object not found!" << std::endl;
        return;
    }
    GameObject* playerObj = playerObjs[0];
    if (playerObj == nullptr) {
        std::cerr << "Player object not found at [0]!" << std::endl;
        return;
    }

    Transform playerTransform = playerObj->getTransform();
    Transform enemyTransform = mGameObject->getTransform();

    int startNode =
        static_cast<int>(enemyTransform.position.y / 16) * mMapWidth + static_cast<int>(enemyTransform.position.x / 16);
    int goalNode = static_cast<int>(playerTransform.position.y / 16) * mMapWidth +
                   static_cast<int>(playerTransform.position.x / 16);

    if (startNode < 0 || startNode >= 50 * 50 || goalNode < 0 || goalNode >= 50 * 50) {
        std::cerr << "Start or Goal node is out of bounds!" << std::endl;
        return;
    }
    std::cout << "Start node: " << startNode << ", Goal node: " << goalNode << std::endl;

    if (!mPathfinding) {
        std::cerr << "Pathfinding object not initialized!" << std::endl;
        return;
    }

    mPath = mPathfinding->findPath(startNode, goalNode);

    std::cout << "Path to player: ";
    for (int node : mPath) {
        std::cout << node << " ";
    }
    std::cout << std::endl;
}
