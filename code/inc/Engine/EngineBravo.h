/**
 * @file EngineBravo.h
 *
 * @brief Header file for the EngineBravo class.
 */
/**
 * @mainpage Engine Bravo
 *
 * Engine Bravo is the game engine, created for the Systems Programming in C++ minor at Avans University of Applied
 * Sciences. It largely uses the interface provided by Unity. Managers contain only references to the game objects which
 * are relevant to them. For example, the physics manager only
 * contains references to game objects with collider or rigidbody components. Whenever a component is added to or
 * removed
 * from a game object, the game object calls a function in the game engine class. This adds the game object to an update
 * queue. This update queue is used every cycle, where the engine checks all components of the object in the queue, and
 * adds or removes them to the relevant managers. Managers have a private constructor and can only be created from the
 * main engine class. This is done to ensure that there is only one instance of each manager.

 */

#pragma once

#include "AudioManager.h"
#include "Engine/ResourceManager.h"
#include "Engine/SaveGameManager.h"
#include "EventManager.h"
#include "Network/NetworkManager.h"
#include "ParticleSystem.h"
#include "PhysicsManager.h"
#include "RenderSystem.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "Time.h"
#include "UIManager.h"
#include "UpdateQueue.h"
#include <queue>

/**
 * @class EngineBravo
 *
 * @brief The main class of the engine. It is a singleton and contains all the managers, which are responsible for the
 * various systems in the engine.
 */
class EngineBravo
{
public:
	static EngineBravo& getInstance();

	void initialize();
	void run();

	void stopEngine();
	void setFrameRateLimit(int aFrameRate);

	SceneManager& getSceneManager();
	RenderSystem& getRenderSystem();
	ResourceManager& getResourceManager();
	ParticleSystem& getParticleSystem();
	NetworkManager& getNetworkManager();
	SaveGameManager& getSaveGameManager();
	AudioManager& getAudioManager();
	EventManager& getEventManager();
	UIManager& getUIManager();
	PhysicsManager& getPhysicsManager();

	UpdateQueue& getUpdateQueue();

private:
	// Private constructor and destructor
	EngineBravo();
	~EngineBravo();

	// Deleted copy constructor and assignment operator to prevent copying
	EngineBravo(const EngineBravo&) = delete;
	EngineBravo& operator=(const EngineBravo&) = delete;

	void runBehaviourScripts();
	void startBehaviourScripts();

	void limitFrameRate(int aFrameRate);

	void handleEvent(const Event& aEvent);

private:
	/// @brief Maximum frame rate (in frames per second)
	int mFrameRateLimit;
	/// @brief Flag to indicate if the engine is running. Is used to stop the engine.
	bool mRunning;

	/// @brief Scene manager: responsible for scene transitions
	SceneManager mSceneManager;
	/// @brief Render system: responsible for rendering objects
	RenderSystem mRenderSystem;
	/// @brief Resource manager: responsible for loading and managing resources
	ResourceManager mResourceManager;
	/// @brief Particle system: responsible for creating and destroying particles
	ParticleSystem mParticleSystem;
	/// @brief Network manager: responsible for network communication
	NetworkManager mNetworkManager;
	/// @brief Event manager: responsible for handling user input events
	EventManager mEventManager;
	/// @brief Save game manager: responsible for saving and loading save games
	SaveGameManager mSaveGameManager;
	/// @brief Audio manager: responsible for playing audio
	AudioManager mAudioManager;
	/// @brief UI manager: responsible for managing UI elements, such as buttons and HUD
	UIManager mUIManager;
	/// @brief Physics manager: responsible for physics calculations
	PhysicsManager mPhysicsManager;

	/// @brief Update queue: responsible for updating the objects in all managers.
	UpdateQueue mUpdateQueue;
};
