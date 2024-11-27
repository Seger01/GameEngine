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
	engineBravo.clearUpdateObjects();

	AudioManager& audioManager = engineBravo.getAudioManager();
	audioManager.clearObjects();

	NetworkManager& networkManager = engineBravo.getNetworkManager();
	PhysicsManager& physicsManager = engineBravo.getPhysicsManager();
	RenderSystem& renderSystem = engineBravo.getRenderSystem();
	UIManager& uiManager = engineBravo.getUIManager();

	GameObject* gameObject = new GameObject();

	engineBravo.updateAdditions();

	ASSERT_EQ(audioManager.getObjects().size(), 0);
	ASSERT_EQ(networkManager.getObjects().size(), 0);
	ASSERT_EQ(physicsManager.getPhysicsEngine().getObjects().size(), 0);
	ASSERT_EQ(renderSystem.getObjects().size(), 0);
	ASSERT_EQ(uiManager.getObjects().size(), 0);

	AudioSource* audioSource = new AudioSource("Audio/gun1.wav");
	gameObject->addComponent(audioSource);
	engineBravo.updateAdditions();

	ASSERT_EQ(audioManager.getObjects().size(), 1);
	ASSERT_EQ(networkManager.getObjects().size(), 0);
	ASSERT_EQ(physicsManager.getPhysicsEngine().getObjects().size(), 0);
	ASSERT_EQ(renderSystem.getObjects().size(), 0);
	ASSERT_EQ(uiManager.getObjects().size(), 0);

	gameObject->removeComponent(audioSource);
	RigidBody* rigidBody = new RigidBody();
	gameObject->addComponent(rigidBody);
	engineBravo.updateAdditions();

	ASSERT_EQ(audioManager.getObjects().size(), 0);
	ASSERT_EQ(networkManager.getObjects().size(), 0);
	ASSERT_EQ(physicsManager.getPhysicsEngine().getObjects().size(), 1);
	ASSERT_EQ(renderSystem.getObjects().size(), 0);
	ASSERT_EQ(uiManager.getObjects().size(), 0);
}

TEST(UpdateQueue, updateRemovals)
{
	EngineBravo& engineBravo = EngineBravo::getInstance();
	engineBravo.clearUpdateObjects();
	SceneManager& sceneManager = engineBravo.getSceneManager();
	sceneManager.createScene("TestScene");
	sceneManager.requestSceneChange("TestScene");
	GameObject* gameObject = new GameObject();

	AudioManager& audioManager = engineBravo.getAudioManager();

	AudioSource* audioSource = new AudioSource("Audio/gun1.wav");
	gameObject->addComponent(audioSource);

	ASSERT_EQ(audioManager.getObjects().size(), 0);

	sceneManager.getCurrentScene()->addGameObject(gameObject);
	engineBravo.addToUpdateObjects(*gameObject);
	engineBravo.updateAdditions();

	ASSERT_EQ(audioManager.getObjects().size(), 1);

	sceneManager.getCurrentScene()->requestGameObjectRemoval(gameObject);
	engineBravo.updateRemovals();
	sceneManager.update();

	ASSERT_EQ(audioManager.getObjects().size(), 0);
}