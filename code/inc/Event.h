#pragma once

#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "SDL.h"

#include "Inputs.h"

enum class EventType;

struct Mouse {
    int x = 0;
    int y = 0;
    bool left = false;
    bool middle = false;
    bool right = false;
};

struct Event {
    EventType eventType;

    Mouse mouse; // to store mouse data in event
    Key key;     // to store which key pressed data in event
    float range; // to store button state range in event

    DefAction defAction = DefAction::Undefined;

    std::string customName = "";
};

// Define an enum class for all keyboard, mouse, and controller keys
enum class EventType {
    KeyUp,
    KeyDown,
    MouseUp,
    MouseDown,

    Quit,

    DefinedAction,
    Custom,

};

class EventManager {
public:
    EventManager() {}

    // Define a type for event callbacks
    using EventCallback = std::function<void(const Event&)>;

    // Subscribe to an event type
    void subscribe(EventType eventType, EventCallback callback) { subscribers[eventType].push_back(callback); }

    // Handle events and call subscribed callbacks
    void handleEvents() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {

            // if (subscribers.find(event.type) != subscribers.end()) {
            //     for (const auto& callback : subscribers[event.type]) {
            //         callback(event);
            //     }
            // }
        }
    }

private:
    // Store subscribers for each event type
    std::unordered_map<EventType, std::vector<EventCallback>> subscribers;
};
