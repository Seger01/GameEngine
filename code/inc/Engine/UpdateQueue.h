/**
 * @file UpdateQueue.h
 *
 * @brief Contains the declaration of the UpdateQueue class
 */

#pragma once

#include <vector>

class GameObject;

/**
 * @brief Class that ensures all managers have updated object references
 */
class UpdateQueue
{
public:
	void addToUpdateObjects(GameObject& aGameObject);
	void clearUpdateObjects();
	void updateAdditions();
	void updateRemovals();
	void clearManagerObjects();

private:
	std::vector<std::reference_wrapper<GameObject>> mUpdateObjects;
};