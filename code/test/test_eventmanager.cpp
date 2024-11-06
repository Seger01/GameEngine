#include "EventManager.h"
#include <SDL.h>
#include <gtest/gtest.h>

// Mock EventCallback function to test callback triggering
void mockCallback(const Event& event, EventType& eventTypeCaptured) { eventTypeCaptured = event.type; }

TEST(EventManagerTest, SubscribeToEvent) {
    EventManager eventManager;
    EventType eventTypeCaptured = EventType::Undefined;

    // Subscribe to KeyDown event
    eventManager.subscribe([&eventTypeCaptured](const Event& event) { mockCallback(event, eventTypeCaptured); },
                           EventType::KeyDown);

    // Dispatch KeyDown event and check if callback is called
    Event testEvent{EventType::KeyDown, {}, {}};
    eventManager.dispatch(testEvent);

    ASSERT_EQ(eventTypeCaptured, EventType::KeyDown);
}

TEST(EventManagerTest, ParseSDLEvent_KeyDown) {
    EventManager eventManager;
    SDL_Event sdlEvent;
    sdlEvent.type = SDL_KEYDOWN;
    sdlEvent.key.keysym.scancode = SDL_SCANCODE_A;

    Event parsedEvent = eventManager.parseSDLEvent(sdlEvent);

    ASSERT_EQ(parsedEvent.type, EventType::KeyDown);
    ASSERT_EQ(parsedEvent.key, (Key)SDL_SCANCODE_A);
}

TEST(EventManagerTest, ParseSDLEvent_MouseButtonDown) {
    EventManager eventManager;
    SDL_Event sdlEvent;
    sdlEvent.type = SDL_MOUSEBUTTONDOWN;
    sdlEvent.button.button = SDL_BUTTON_LEFT;
    sdlEvent.motion.x = 100;
    sdlEvent.motion.y = 150;

    Event parsedEvent = eventManager.parseSDLEvent(sdlEvent);

    ASSERT_EQ(parsedEvent.type, EventType::MouseButtonDown);
    ASSERT_TRUE(parsedEvent.mouse.left);
    ASSERT_EQ(parsedEvent.mouse.position.x, 100);
    ASSERT_EQ(parsedEvent.mouse.position.y, 150);
}

TEST(EventManagerTest, HandleEvents) {
    // Initialize SDL with event system
    ASSERT_EQ(SDL_Init(SDL_INIT_EVENTS), 0); // Make sure SDL initializes successfully

    EventManager eventManager;
    EventType eventTypeCaptured = EventType::Undefined;

    // Subscribe to Quit event
    eventManager.subscribe([&eventTypeCaptured](const Event& event) { mockCallback(event, eventTypeCaptured); },
                           EventType::Quit);

    // Create an SDL quit event and push it to the event queue
    SDL_Event sdlEvent;
    sdlEvent.type = SDL_QUIT;
    SDL_PushEvent(&sdlEvent);

    // Handle the events
    eventManager.handleEvents();

    ASSERT_EQ(eventTypeCaptured, EventType::Quit);
}

TEST(EventManagerTest, DispatchCallsCorrectCallbacks) {
    EventManager eventManager;
    EventType keyUpCaptured = EventType::Undefined;
    EventType mouseButtonDownCaptured = EventType::Undefined;

    // Subscribe to KeyUp and MouseButtonDown events
    eventManager.subscribe([&keyUpCaptured](const Event& event) { mockCallback(event, keyUpCaptured); },
                           EventType::KeyUp);

    eventManager.subscribe(
        [&mouseButtonDownCaptured](const Event& event) { mockCallback(event, mouseButtonDownCaptured); },
        EventType::MouseButtonDown);

    // Dispatch KeyUp event
    Event keyUpEvent{EventType::KeyUp, {}, {}};
    eventManager.dispatch(keyUpEvent);

    ASSERT_EQ(keyUpCaptured, EventType::KeyUp);
    ASSERT_EQ(mouseButtonDownCaptured, EventType::Undefined);

    // Dispatch MouseButtonDown event
    Event mouseButtonDownEvent{EventType::MouseButtonDown, {}, {}};
    eventManager.dispatch(mouseButtonDownEvent);

    ASSERT_EQ(mouseButtonDownCaptured, EventType::MouseButtonDown);
}
