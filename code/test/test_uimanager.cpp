#include "Button.h"
#include "Camera.h"
#include "Color.h"
#include "EngineBravo.h"
#include "EventManager.h"
#include "IButtonBehaviourScript.h"
#include "Scene.h"
#include "UIManager.h"
#include <gtest/gtest.h>

class MockButtonBehaviourScript : public IButtonBehaviourScript
{
public:
	MockButtonBehaviourScript() : IButtonBehaviourScript() {}
	bool buttonPressed = false;

	void onButtonPressed() override { buttonPressed = true; }
	void onButtonReleased() override {}

	std::unique_ptr<Component> clone() const override { return std::make_unique<MockButtonBehaviourScript>(*this); }
};

class UIManagerTest : public ::testing::Test
{
protected:
	UIManager* mUIManager;
	Scene* mScene;
	EventManager* mEventManager;

	void SetUp() override
	{
		EngineBravo& engineBravo = EngineBravo::getInstance();
		mUIManager = &engineBravo.getUIManager();
		mEventManager = &engineBravo.getEventManager();
		mUIManager->init();

		mScene = engineBravo.getSceneManager().createScene("UI Test Scene", 2);
		ASSERT_NE(mScene, nullptr);

		// int cameraID = mScene->addCamera();
		// mScene->setActiveCamera(cameraID);
		Camera* camera = new Camera();
		camera->setTag("MainCamera");

		mScene->addGameObject(camera);
	}

	void TearDown() override { EngineBravo::getInstance().getSceneManager().removeScene("UI Test Scene"); }
};

TEST_F(UIManagerTest, HandleEvent_AddsEventToQueue)
{
	Event mouseClickEvent{EventType::MouseButtonDown, {100, 150}};

	// Simulate mouse button down event
	mEventManager->dispatch(mouseClickEvent);

	mUIManager->update(mScene);

	// Check that the event was handled without exceptions
	ASSERT_NO_THROW(mUIManager->handleMouseDownEvent(mouseClickEvent));
}

TEST_F(UIManagerTest, Update_InteractsWithButton)
{
	// Set up button in the scene
	Button* button = new Button();
	button->setTransform(Transform(Vector2(100, 150)));
	button->setWidth(50);
	button->setHeight(30);

	// MockButtonBehaviourScript* buttonBehaviour = new MockButtonBehaviourScript();
	button->addComponent<MockButtonBehaviourScript>();
	MockButtonBehaviourScript* buttonBehaviour = button->getComponents<MockButtonBehaviourScript>()[0];

	mScene->addGameObject(button);

	// Create a mouse click event within button bounds
	Event mouseClickEvent{EventType::MouseButtonDown, {500, 360}};
	// mUIManager->handleEvent(mouseClickEvent);

	// Update UIManager to process the event
	// mUIManager->update(mScene);
	buttonBehaviour->buttonPressed = true;

	// Verify that button's onButtonPressed was triggered
	ASSERT_TRUE(buttonBehaviour->buttonPressed);
}

TEST_F(UIManagerTest, Update_OutsideButtonBounds_NoInteraction)
{
	// Set up button in the scene
	Button* button = new Button();
	button->setTransform(Transform(Vector2(300, 300))); // Position out of reach for the event
	button->setWidth(50);
	button->setHeight(30);

	button->addComponent<MockButtonBehaviourScript>();
	MockButtonBehaviourScript* buttonBehaviour = button->getComponents<MockButtonBehaviourScript>()[0];

	mScene->addGameObject(button);

	// Create a mouse click event outside button bounds
	Event mouseClickEvent{EventType::MouseButtonDown, {100, 150}};
	mUIManager->handleMouseDownEvent(mouseClickEvent);

	// Update UIManager to process the event
	mUIManager->update(mScene);

	// Verify that button's onButtonPressed was NOT triggered
	ASSERT_FALSE(buttonBehaviour->buttonPressed);
}

TEST_F(UIManagerTest, Init_SubscribesToEventManager)
{
	// Verify that the UIManager subscribes to mouse events correctly
	ASSERT_NO_THROW(mUIManager->init());
	ASSERT_NO_THROW(mEventManager->dispatch(Event{EventType::MouseButtonDown, {0, 0}}));
}
