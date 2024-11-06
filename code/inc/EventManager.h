#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <functional>
#include <unordered_map>
#include <vector>

#include "Input/InputStructs.h"
#include "SDL_events.h"
#include "SDL_keycode.h"

// Define an enum class for all keyboard, mouse, and controller keys
enum class EventType {
    KeyUp = 0,
    KeyDown,

    MouseButtonUp,
    MouseButtonDown,
    MouseMove,

    Quit,

    Undefined,
};

struct Event {
    EventType type;

    Mouse mouse; // to store mouse data in event
    Key key;     // to store which key pressed data in event
};

class EventManager {
public:
    EventManager() {}

    // Define a type for event callbacks
    using EventCallback = std::function<void(const Event&)>;

    // Subscribe to an event type
    void subscribe(EventCallback callback, EventType eventType);

    Key SDLKeyToOwnKey(SDL_Scancode SDLKeyCode);

    Mouse parseSDLMouse(const SDL_Event& aEvent);

    Event parseSDLEvent(const SDL_Event& aEvent);

    // Handle events and call subscribed callbacks
    void handleEvents();

    void dispatch(Event aEvent);

private:
    // Store subscribers for each event type
    std::unordered_map<EventType, std::vector<EventCallback>> subscribers;
};

#endif
