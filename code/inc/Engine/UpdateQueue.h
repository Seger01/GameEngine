#pragma once

#include <vector>

class GameObject;

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