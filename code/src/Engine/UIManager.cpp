/**
 * @file UIManager.cpp
 * @brief UIManager class implementation file.
 */

#include "Engine/UIManager.h"

#include "Components/IButtonBehaviourScript.h"
#include "Engine/EngineBravo.h"
#include "EventManager.h"
#include "Input/Input.h"
#include "Scene.h"
#include "UI/Button.h"

/**
 * @brief UIManager class constructor
 */
UIManager::UIManager() {}

/**
 * @brief UIManager class destructor
 */
UIManager::~UIManager() {}

/**
 * @brief Initialize the UIManager and subscribe to the required events
 */
void UIManager::init()
{
	EventManager& eventManager = EngineBravo::getInstance().getEventManager();

	// eventManager.subscribe(std::bind(&UIManager::handleMouseDownEvent, this, std::placeholders::_1),
	// 					   EventType::MouseButtonDown);
	// eventManager.subscribe(std::bind(&UIManager::handleMouseUpEvent, this, std::placeholders::_1),
	// 					   EventType::MouseButtonUp);
}

/**
 * @brief Handle the mouse down event
 * @param aEvent The mouse down event
 */
void UIManager::handleMouseDownEvent(const Event& aEvent) { mMouseDownEventQueue.push_back(aEvent); }

/**
 * @brief Handle the mouse up event
 * @param aEvent The mouse up event
 */
void UIManager::handleMouseUpEvent(const Event& aEvent) { mMouseUpEventQueue.push_back(aEvent); }

/**
 * @brief Update the UIManager
 * @param aScene The current scene
 */
void UIManager::update(const Scene& aScene)
{
	for (GameObject& gameObject : mObjects)
	{
		Button& button = dynamic_cast<Button&>(gameObject);

		Camera* currentCamera = aScene.getMainCamera();
		if (currentCamera == nullptr)
		{
			return;
		}
		Input& input = Input::getInstance();
		if (input.GetMouseButtonDown(MouseButton::LEFT))
		{
			Point mouseScreenPos = input.MousePosition();

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

		if (input.GetMouseButtonUp(MouseButton::LEFT))
		{
			Point mouseScreenPos = input.MousePosition();

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

/**
 * @brief Add a UI object to the UIManager
 * @param aObject The UI object to add
 */
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

/**
 * @brief Remove a UI object from the UIManager
 * @param aObject The UI object to remove
 */
void UIManager::removeObject(GameObject& aObject)
{
	auto it = std::remove_if(mObjects.begin(), mObjects.end(), [&aObject](const std::reference_wrapper<GameObject>& obj)
							 { return &obj.get() == &aObject; });
	if (it != mObjects.end())
	{
		mObjects.erase(it, mObjects.end());
	}
}

/**
 * @brief Get the list of UI objects
 * @return The list of UI objects
 */
const std::vector<std::reference_wrapper<GameObject>>& UIManager::getObjects() const { return mObjects; }

/**
 * @brief Clear the list of UI objects
 */
void UIManager::clearObjects() { mObjects.clear(); }
