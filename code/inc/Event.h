#ifndef EVENT_H
#define EVENT_H

#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "SDL.h"

// #include "ContextManager.h"
#include "Input.h"
#include "InputStructs.h"
#include "SDL_events.h"
#include "SDL_keycode.h"

// Define an enum class for all keyboard, mouse, and controller keys
enum class EventType {
    KeyUp,
    KeyDown,
    KeyHeld,

    MouseButtonUp,
    MouseButtonDown,
    MouseMove,

    Quit,

    DefinedAction,
    Custom,

    Undefined,
    Any,
};

struct Event {
    EventType type;

    Mouse mouse; // to store mouse data in event
    Key key;     // to store which key pressed data in event
    float range; // to store button state range in event

    DefAction defAction = DefAction::Undefined;

    std::string customName = "";
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
        mouse.position.x = aEvent.motion.x;
        mouse.position.y = aEvent.motion.y;

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
        // SDL_Event event;
        //
        // static Input& input = Input::getInstance();
        //
        // std::vector<Uint8> heldKeys = input.getHeldKeys();
        // std::vector<Uint8> downKeys = input.getDownKeys();
        // std::vector<Uint8> upKeys = input.getUpKeys();
        //
        // for (int i = 0; i < heldKeys.size(); i++) {
        //     Event createdEvent;
        //     createdEvent.type = EventType::KeyHeld;
        //     createdEvent.key = (Key)heldKeys[i];
        //     dispatch(createdEvent);
        // }
        //
        // for (int i = 0; i < downKeys.size(); i++) {
        //     Event createdEvent;
        //     createdEvent.type = EventType::KeyDown;
        //     createdEvent.key = (Key)downKeys[i];
        //     dispatch(createdEvent);
        // }
        //
        // for (int i = 0; i < upKeys.size(); i++) {
        //     Event createdEvent;
        //     createdEvent.type = EventType::KeyUp;
        //     createdEvent.key = (Key)upKeys[i];
        //     dispatch(createdEvent);
        // }
        //
        // if (input.GetMouseButtonDown(MouseButton::LEFT)) {
        //     Event createdEvent;
        //
        //     createdEvent.type = EventType::MouseButtonDown;
        //     createdEvent.mouse.left = true;
        //
        //     dispatch(createdEvent);
        // }
        //
        // if (input.GetMouseButtonDown(MouseButton::MIDDLE)) {
        //     Event createdEvent;
        //
        //     createdEvent.type = EventType::MouseButtonDown;
        //     createdEvent.mouse.middle = true;
        //
        //     dispatch(createdEvent);
        // }
        //
        // if (input.GetMouseButtonDown(MouseButton::RIGHT)) {
        //     Event createdEvent;
        //
        //     createdEvent.type = EventType::MouseButtonDown;
        //     createdEvent.mouse.right = true;
        //
        //     dispatch(createdEvent);
        // }
        //
        // // for (int i = 0; i < heldKeys.size(); i++) {
        // //     if (mContextManager.isKeyActive((Key)heldKeys[i]) != DefAction::Undefined) {
        // //         Event createdEvent;
        // //         createdEvent.type = EventType::DefinedAction;
        // //         createdEvent.defAction = mContextManager.isKeyActive((Key)heldKeys[i]);
        // //
        // //         createdEvent.key = (Key)heldKeys[i];
        // //         dispatch(createdEvent);
        // //     }
        // // }
        // //
        // for (int i = 0; i < downKeys.size(); i++) {
        //     Event createdEvent;
        //     createdEvent.type = EventType::KeyDown;
        //     createdEvent.key = (Key)downKeys[i];
        //     dispatch(createdEvent);
        // }
        //
        // for (int i = 0; i < upKeys.size(); i++) {
        //     Event createdEvent;
        //     createdEvent.type = EventType::KeyUp;
        //     createdEvent.key = (Key)upKeys[i];
        //     dispatch(createdEvent);
        // }
        //
        // // while (SDL_PollEvent(&event)) {
        // //     Event ownEvent = this->parseSDLEvent(event);
        // //
        // // }
    }

    void dispatch(Event aEvent) {
        std::cout << "dispatch(Event) call" << std::endl;
        if (subscribers.find(aEvent.type) != subscribers.end()) {
            for (const auto& callback : subscribers[aEvent.type]) {
                callback(aEvent);
            }
        }
        if (subscribers.find(EventType::Any) != subscribers.end()) {
            for (const auto& callback : subscribers[EventType::Any]) {
                callback(aEvent);
            }
        }
    }

private:
    ContextManager mContextManager;
    // Store subscribers for each event type
    std::unordered_map<EventType, std::vector<EventCallback>> subscribers;
};

#endif
