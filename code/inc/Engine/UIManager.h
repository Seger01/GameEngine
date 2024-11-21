#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <iostream>
#include <string>
#include <vector>

#include "EventManager.h"
#include "Scene.h"
#include "UIObject.h"

class UIManager {
public:
    friend class EngineBravo;

    void init();
    void update(Scene* aScene);

    void handleMouseDownEvent(const Event& aEvent);
    void handleMouseUpEvent(const Event& aEvent);

private:
    // std::vector<UIObject&> mClickableButtons;
    UIManager();
    ~UIManager();

    std::vector<Event> mMouseDownEventQueue;
    std::vector<Event> mMouseUpEventQueue;
};

#endif // UIMANAGER_H
