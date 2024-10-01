#pragma once

#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "SDL.h"

#include "Inputs.h"
#include "SDL_events.h"
#include "SDL_keycode.h"

enum class EventType;

struct Mouse {
    int x = 0;
    int y = 0;
    bool left = false;
    bool middle = false;
    bool right = false;
};

struct Event {
    EventType type;

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

    MouseButtonUp,
    MouseButtonDown,
    MouseMove,

    Quit,

    DefinedAction,
    Custom,

    Undefined,
    Any,
};

class EventManager {
public:
    EventManager() {}

    // Define a type for event callbacks
    using EventCallback = std::function<void(const Event&)>;

    // Subscribe to an event type
    void subscribe(EventCallback callback, EventType eventType = EventType::Any) {
        subscribers[eventType].push_back(callback);
    }

    Key SDLKeyToOwnKey(SDL_Scancode SDLKeyCode) { return (Key)SDLKeyCode; }

    Mouse parseSDLMouse(const SDL_Event& aEvent) {
        Mouse mouse;

        if (aEvent.button.button == SDL_BUTTON_LEFT) {
            mouse.left = true;
        } else if (aEvent.button.button == SDL_BUTTON_RIGHT) {
            mouse.right = true;
        } else if (aEvent.button.button == SDL_BUTTON_MIDDLE) {
            mouse.middle = true;
        }
        mouse.x = aEvent.motion.x;
        mouse.y = aEvent.motion.y;

        return mouse;
    }

    Event parseSDLEvent(const SDL_Event& aEvent) {
        Event createdEvent;
        switch (aEvent.type) {
        case (SDL_KEYDOWN):
            createdEvent.type = EventType::KeyDown;
            createdEvent.key = SDLKeyToOwnKey(aEvent.key.keysym.scancode);
            break;
        case (SDL_KEYUP):
            createdEvent.type = EventType::KeyUp;
            createdEvent.key = SDLKeyToOwnKey(aEvent.key.keysym.scancode);
            break;

        case (SDL_MOUSEBUTTONDOWN):
            createdEvent.type = EventType::MouseButtonDown;
            createdEvent.mouse = parseSDLMouse(aEvent);
            break;
        case (SDL_MOUSEBUTTONUP):
            createdEvent.type = EventType::MouseButtonUp;
            createdEvent.mouse = parseSDLMouse(aEvent);
            break;
        case (SDL_MOUSEMOTION):
            createdEvent.type = EventType::MouseMove;
            createdEvent.mouse = parseSDLMouse(aEvent);
            break;

        case (SDL_QUIT):
            createdEvent.type = EventType::Quit;
            break;
        }

        return createdEvent;
    }

    // Handle events and call subscribed callbacks
    void handleEvents() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            Event ownEvent = this->parseSDLEvent(event);

            if (subscribers.find(ownEvent.type) != subscribers.end()) {
                for (const auto& callback : subscribers[ownEvent.type]) {
                    callback(ownEvent);
                }
            }
            if (subscribers.find(EventType::Any) != subscribers.end()) {
                for (const auto& callback : subscribers[EventType::Any]) {
                    callback(ownEvent);
                }
            }
        }
    }

private:
    // Store subscribers for each event type
    std::unordered_map<EventType, std::vector<EventCallback>> subscribers;
};
