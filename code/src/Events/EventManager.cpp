#include "Events/EventManager.h"

void EventManager::subscribe(EventCallback callback, EventType eventType)
{
	subscribers[eventType].push_back(callback);
}

Key EventManager::SDLKeyToOwnKey(SDL_Scancode SDLKeyCode) { return (Key)SDLKeyCode; }

Mouse EventManager::parseSDLMouse(const SDL_Event& aEvent)
{
	Mouse mouse;

	if (aEvent.button.button == SDL_BUTTON_LEFT)
	{
		mouse.left = true;
	}
	else if (aEvent.button.button == SDL_BUTTON_RIGHT)
	{
		mouse.right = true;
	}
	else if (aEvent.button.button == SDL_BUTTON_MIDDLE)
	{
		mouse.middle = true;
	}
	mouse.position.x = aEvent.motion.x;
	mouse.position.y = aEvent.motion.y;

	return mouse;
}

Event EventManager::parseSDLEvent(const SDL_Event& aEvent)
{
	Event createdEvent;
	switch (aEvent.type)
	{
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

void EventManager::handleEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		Event createdEvent = parseSDLEvent(event);
		dispatch(createdEvent);
	}
}

void EventManager::dispatch(Event aEvent)
{
	if (subscribers.find(aEvent.type) != subscribers.end())
	{
		for (const auto& callback : subscribers[aEvent.type])
		{
			callback(aEvent);
		}
	}
}
