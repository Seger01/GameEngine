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

    void handleEvent(const Event& aEvent);

private:
    // std::vector<UIObject&> mClickableButtons;
    UIManager();
    ~UIManager();

    std::vector<Event> mEventQueue;
};

#endif // UIMANAGER_H
