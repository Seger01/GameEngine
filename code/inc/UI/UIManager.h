/**
 * @file UIManager.h
 * @brief UIManager class header file.
 */

#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <iostream>
#include <string>
#include <vector>

#include "Events/EventManager.h"
#include "Scenes/Scene.h"
#include "UI/UIObject.h"

/**
 * @class UIManager
 * @brief UIManager class responsible for calling the required function on the UI objects.
 */

class UIManager
{
public:
	friend class EngineBravo;

	void init();
	void update(const Scene& aScene);

	void handleMouseDownEvent(const Event& aEvent);
	void handleMouseUpEvent(const Event& aEvent);

public:
	void addObject(GameObject& aObject);
	void removeObject(GameObject& aObject);
	const std::vector<std::reference_wrapper<GameObject>>& getObjects() const;
	void clearObjects();

private:
	/// @brief The list of UI objects
	std::vector<std::reference_wrapper<GameObject>> mObjects;

	/// @brief the constructor is private to prevent instantiation
	UIManager();
	/// @brief the destructor is private to prevent instantiation
	~UIManager();

	/// @brief The mouse down event queue which gets processed when update is called
	std::vector<Event> mMouseDownEventQueue;
	/// @brief The mouse up event queue which gets processed when update is called
	std::vector<Event> mMouseUpEventQueue;
};

#endif // UIMANAGER_H
