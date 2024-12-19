/**
 * @mainpage Engine Bravo
 *
 * Engine Bravo is the game engine, created for the Systems Programming in C++ minor at Avans University of Applied
 * Sciences. It largely uses the interface provided by Unity.
 *
 */

#ifndef ENGINEBRAVO_H
#define ENGINEBRAVO_H

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

class EngineBravo
{
public:
	// Get the single instance of EngineBravo
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
	int mFrameRateLimit;
	bool mRunning;

	SceneManager mSceneManager;
	RenderSystem mRenderSystem;
	ResourceManager mResourceManager;
	ParticleSystem mParticleSystem;
	NetworkManager mNetworkManager;
	EventManager mEventManager;
	SaveGameManager mSaveGameManager;
	AudioManager mAudioManager;
	UIManager mUIManager;
	PhysicsManager mPhysicsManager;

	UpdateQueue mUpdateQueue;
};

#endif // ENGINEBRAVO_H
