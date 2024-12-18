#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <iostream>
#include <string>
#include <vector>

#include "EventManager.h"
#include "Scene.h"
#include "UIObject.h"

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
	std::vector<std::reference_wrapper<GameObject>> mObjects;
	// std::vector<UIObject&> mClickableButtons;
	UIManager();
	~UIManager();

	std::vector<Event> mMouseDownEventQueue;
	std::vector<Event> mMouseUpEventQueue;
};

#endif // UIMANAGER_H
