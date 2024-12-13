#include "Engine/UIManager.h"

#include "Button.h"
#include "Components/IButtonBehaviourScript.h"
#include "Engine/EngineBravo.h"
#include "EventManager.h"
#include "Input.h"
#include "Scene.h"

UIManager::UIManager() {}

UIManager::~UIManager() {}

void UIManager::init()
{
	EventManager& eventManager = EngineBravo::getInstance().getEventManager();

	eventManager.subscribe(std::bind(&UIManager::handleMouseDownEvent, this, std::placeholders::_1),
						   EventType::MouseButtonDown);
	eventManager.subscribe(std::bind(&UIManager::handleMouseUpEvent, this, std::placeholders::_1),
						   EventType::MouseButtonUp);
}

void UIManager::handleMouseDownEvent(const Event& aEvent) { mMouseDownEventQueue.push_back(aEvent); }

void UIManager::handleMouseUpEvent(const Event& aEvent) { mMouseUpEventQueue.push_back(aEvent); }

void UIManager::update(const Scene& aScene)
{
	for (GameObject& gameObject : mObjects)
	{
		Button& button = dynamic_cast<Button&>(gameObject);

		Camera* currentCamera = aScene.getCameraWithTag("MainCamera");

		for (Event event : mMouseDownEventQueue)
		{
			Point mouseScreenPos = event.mouse.position;

			Vector2 worldMousePos =
				EngineBravo::getInstance().getRenderSystem().screenToWorldPos(mouseScreenPos, *currentCamera);

			if (button.interactable())
			{
				if (worldMousePos.x >= button.getTransform().position.x &&
					worldMousePos.x <= button.getTransform().position.x + button.getWidth() &&
					worldMousePos.y >= button.getTransform().position.y &&
					worldMousePos.y <= button.getTransform().position.y + button.getHeight())
				{
					if (button.getComponents<IButtonBehaviourScript>().size() > 0)
					{
						if (event.type == EventType::MouseButtonDown)
						{
							for (IButtonBehaviourScript& buttonBehaviourScript :
								 button.getComponents<IButtonBehaviourScript>())
							{
								buttonBehaviourScript.onButtonPressed();
								button.activateOnClickCallback();
							}
						}
					}
				}
			}
		}

		for (Event event : mMouseUpEventQueue)
		{
			Point mouseScreenPos = event.mouse.position;

			Vector2 worldMousePos =
				EngineBravo::getInstance().getRenderSystem().screenToWorldPos(mouseScreenPos, *currentCamera);

			if (button.interactable())
			{
				if (worldMousePos.x >= button.getTransform().position.x &&
					worldMousePos.x <= button.getTransform().position.x + button.getWidth() &&
					worldMousePos.y >= button.getTransform().position.y &&
					worldMousePos.y <= button.getTransform().position.y + button.getHeight())
				{
					if (button.getComponents<IButtonBehaviourScript>().size() > 0)
					{
						if (event.type == EventType::MouseButtonUp)
						{
							for (IButtonBehaviourScript& buttonBehaviourScript :
								 button.getComponents<IButtonBehaviourScript>())
							{
								buttonBehaviourScript.onButtonReleased();
								button.activateOnReleaseCallback();
							}
						}
					}
				}
			}
		}

		Point currentMousePos = Input::getInstance().MousePosition();

		Vector2 worldMousePos =
			EngineBravo::getInstance().getRenderSystem().screenToWorldPos(currentMousePos, *currentCamera);

		if (button.interactable())
		{
			if (worldMousePos.x >= button.getTransform().position.x &&
				worldMousePos.x <= button.getTransform().position.x + button.getWidth() &&
				worldMousePos.y >= button.getTransform().position.y &&
				worldMousePos.y <= button.getTransform().position.y + button.getHeight())
			{
				if (button.getComponents<IButtonBehaviourScript>().size() > 0)
				{
					for (IButtonBehaviourScript& buttonBehaviourScript : button.getComponents<IButtonBehaviourScript>())
					{
						if (!button.isHovered())
						{
							buttonBehaviourScript.onButtonHover();
							button.setHovered(true);
						}
					}
				}
			}
			else
			{
				if (button.getComponents<IButtonBehaviourScript>().size() > 0)
				{
					for (IButtonBehaviourScript& buttonBehaviourScript : button.getComponents<IButtonBehaviourScript>())
					{
						if (button.isHovered())
						{
							buttonBehaviourScript.onButtonUnhover();
							button.setHovered(false);
						}
					}
				}
			}
		}
	}
	mMouseDownEventQueue.clear();
	mMouseUpEventQueue.clear();
}

void UIManager::addObject(GameObject& aObject)
{
	auto it = std::find_if(mObjects.begin(), mObjects.end(),
						   [&aObject](const std::reference_wrapper<GameObject>& wrapper)
						   {
							   return &wrapper.get() == &aObject; // Compare addresses
						   });
	if (it == mObjects.end())
	{
		// Object has not been added yet
		mObjects.push_back(aObject);
	}
}

void UIManager::removeObject(GameObject& aObject)
{
	auto it = std::remove_if(mObjects.begin(), mObjects.end(), [&aObject](const std::reference_wrapper<GameObject>& obj)
							 { return &obj.get() == &aObject; });
	if (it != mObjects.end())
	{
		mObjects.erase(it, mObjects.end());
	}
}

const std::vector<std::reference_wrapper<GameObject>>& UIManager::getObjects() const { return mObjects; }

void UIManager::clearObjects() { mObjects.clear(); }
