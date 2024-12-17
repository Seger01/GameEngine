#include "AudioManager.h"
#include "EngineBravo.h"
#include "NetworkManager.h"
#include "PhysicsManager.h"
#include "RenderSystem.h"
#include "UIManager.h"
#include <gtest/gtest.h>

TEST(UpdateQueue, updateAdditions)
{
	EngineBravo& engineBravo = EngineBravo::getInstance();
	engineBravo.getUpdateQueue().clearUpdateObjects();

	AudioManager& audioManager = engineBravo.getAudioManager();
	audioManager.clearObjects();

	NetworkManager& networkManager = engineBravo.getNetworkManager();
	PhysicsManager& physicsManager = engineBravo.getPhysicsManager();
	RenderSystem& renderSystem = engineBravo.getRenderSystem();
	UIManager& uiManager = engineBravo.getUIManager();
	engineBravo.getSceneManager().createScene("TestScene");
	engineBravo.getSceneManager().requestSceneChange("TestScene");

	GameObject* gameObject = new GameObject();
	engineBravo.getSceneManager().getCurrentScene().addGameObject(gameObject);

	engineBravo.getUpdateQueue().updateAdditions();

	ASSERT_EQ(audioManager.getObjects().size(), 0);
	ASSERT_EQ(networkManager.getObjects().size(), 0);
	ASSERT_EQ(physicsManager.getPhysicsEngine().getObjects().size(), 0);
	ASSERT_EQ(renderSystem.getObjects().size(), 0);
	ASSERT_EQ(uiManager.getObjects().size(), 0);

	AudioSource* audioSource = new AudioSource("Audio/gun1.wav");
	gameObject->addComponent(audioSource);
	engineBravo.getUpdateQueue().updateAdditions();

	ASSERT_EQ(audioManager.getObjects().size(), 1);
	ASSERT_EQ(networkManager.getObjects().size(), 0);
	ASSERT_EQ(physicsManager.getPhysicsEngine().getObjects().size(), 0);
	ASSERT_EQ(renderSystem.getObjects().size(), 0);
	ASSERT_EQ(uiManager.getObjects().size(), 0);

	gameObject->removeComponent(audioSource);
	RigidBody* rigidBody = new RigidBody();
	gameObject->addComponent(rigidBody);
	engineBravo.getUpdateQueue().updateAdditions();

	ASSERT_EQ(audioManager.getObjects().size(), 0);
	ASSERT_EQ(networkManager.getObjects().size(), 0);
	ASSERT_EQ(physicsManager.getPhysicsEngine().getObjects().size(), 1);
	ASSERT_EQ(renderSystem.getObjects().size(), 0);
	ASSERT_EQ(uiManager.getObjects().size(), 0);
}

TEST(UpdateQueue, updateRemovals)
{
	EngineBravo& engineBravo = EngineBravo::getInstance();
	engineBravo.getUpdateQueue().clearUpdateObjects();
	engineBravo.getUpdateQueue().clearManagerObjects();
	SceneManager& sceneManager = engineBravo.getSceneManager();
	sceneManager.createScene("TestScene");
	sceneManager.requestSceneChange("TestScene");
	GameObject* gameObject = new GameObject();

	AudioManager& audioManager = engineBravo.getAudioManager();

	AudioSource* audioSource = new AudioSource("Audio/gun1.wav");
	gameObject->addComponent(audioSource);

	ASSERT_EQ(audioManager.getObjects().size(), 0);

	sceneManager.getCurrentScene().addGameObject(gameObject);
	engineBravo.getUpdateQueue().addToUpdateObjects(*gameObject);
	engineBravo.getUpdateQueue().updateAdditions();

	ASSERT_EQ(audioManager.getObjects().size(), 1);

	sceneManager.getCurrentScene().requestGameObjectRemoval(gameObject);
	engineBravo.getUpdateQueue().updateRemovals();
	sceneManager.update();

	ASSERT_EQ(audioManager.getObjects().size(), 0);
}