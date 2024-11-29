#pragma once

#include <vector>

class EngineBravo;
class GameObject;

class UpdateQueue
{
public:
	UpdateQueue();

public:
	void addToUpdateObjects(GameObject& aGameObject);
	void clearUpdateObjects();
	void updateAdditions();
	void updateRemovals();
	void clearManagerObjects();

private:
	EngineBravo& mEngine;
	std::vector<std::reference_wrapper<GameObject>> mUpdateObjects;
};