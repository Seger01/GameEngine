/**
 * @file EngineBravo.cpp
 *
 * @brief Implementation of the EngineBravo class.
 */
#include "Engine/EngineBravo.h"

#include <SDL.h>
#include <chrono>
#include <thread>

#include "box2d/types.h"
#include "slikenet/sleep.h"

#include "Animation.h"
#include "Components/IBehaviourScript.h"
#include "Components/ParticleEmitter.h"
#include "Components/Sprite.h"
#include "Global/ScopedTimer.h"
#include "Input/Input.h"
#include "Network/NetworkObject.h"
#include "Renderer.h"
#include "UI/Text.h"

/**
 * @brief Construct a new EngineBravo::EngineBravo object. Sets framre rate limit to 60.
 */
EngineBravo::EngineBravo() : mFrameRateLimit(60), mRunning(false) {}

EngineBravo::~EngineBravo() {}

/**
 * @brief Get the singleton instance of the EngineBravo class. If it is not yet instantiated, it will be created.
 *
 * @return EngineBravo& The singleton instance of the EngineBravo class.
 */
EngineBravo& EngineBravo::getInstance()
{
	static EngineBravo instance;
	return instance;
}

/**
 * @brief Initialize the engine. Sets the renderer for the resource manager, updates the scene manager, initializes the
 * network manager, initializes the time, initializes the UI manager, starts the physics engine.
 */
void EngineBravo::initialize()
{
	mResourceManager.setRenderer(&mRenderSystem.getRenderer());

	mSceneManager.update();

	mNetworkManager.initialize();

	Time::initialize();

	mUIManager.init();

	mPhysicsManager.startPhysicsEngine(Vector2(0, 0.0f));
}

/**
 * @brief Run the engine. This function is the main loop of the engine. It handles input, updates the scene, runs
 * behaviour scripts, updates the physics engine, updates the particle system, renders the scene, updates the network
 * manager, limits the frame rate.
 */
void EngineBravo::run()
{
	Input& input = Input::getInstance();

	mRunning = true;

	mEventManager.subscribe([this](const Event& aEvent) { handleEvent(aEvent); }, EventType::Quit);

	mRenderSystem.getWindow().showWindow();

	mSceneManager.update();

	double accumalatedTimePhysics = 0;

	while (mRunning)
	{
		Time::update();
		accumalatedTimePhysics += Time::rawDeltaTime;

		mEventManager.handleEvents();
		input.update();

		mUIManager.update(mSceneManager.getCurrentScene());

		mSceneManager.update();

		startBehaviourScripts();
		runBehaviourScripts();

		mUpdateQueue.updateAdditions();

		while (accumalatedTimePhysics >= 0.02)
		{
			mPhysicsManager.updatePhysicsEngine(Time::timeDilation);
			accumalatedTimePhysics -= 0.02;
		}

		mParticleSystem.update();
		mRenderSystem.render(mSceneManager.getCurrentScene());

		mNetworkManager.update();

		limitFrameRate(mFrameRateLimit);

		mUpdateQueue.updateRemovals();
	}
}

/**
 * @brief Stop the engine. Sets the running flag to false.
 */
void EngineBravo::stopEngine() { mRunning = false; }

void EngineBravo::setFrameRateLimit(int aFrameRate) { mFrameRateLimit = aFrameRate; }

/**
 * @brief Handle an event. This function is called when an event is triggered.
 *
 * @param aEvent The event to handle.
 */
void EngineBravo::handleEvent(const Event& aEvent)
{
	switch (aEvent.type)
	{
	case EventType::Quit:
		mRunning = false;
		break;
	default:
		break;
	}
}

/**
 * @brief function that limits frame rate by keeping track of the time it takes to render a frame and delaying the next
 * frame if it renders too quickly
 */
void EngineBravo::limitFrameRate(int aFrameRate)
{
	// Minimum time per frame in milliseconds
	int mMinFrameTimeMs = 1000 / aFrameRate;

	// Record the start time of the frame
	static auto frameStart = std::chrono::high_resolution_clock::now();

	// Calculate the time it took to render the frame
	auto frameEnd = std::chrono::high_resolution_clock::now();
	auto frameDurationMs = std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - frameStart).count();

	// Calculate the delay needed to achieve the target frame rate
	int delayTime = mMinFrameTimeMs - frameDurationMs;

	// Delay if the frame rendered too quickly
	if (delayTime > 0)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(delayTime));
	}
	frameStart = std::chrono::high_resolution_clock::now();
}

SceneManager& EngineBravo::getSceneManager() { return mSceneManager; }

RenderSystem& EngineBravo::getRenderSystem() { return mRenderSystem; }

ResourceManager& EngineBravo::getResourceManager() { return mResourceManager; }

ParticleSystem& EngineBravo::getParticleSystem() { return mParticleSystem; }

SaveGameManager& EngineBravo::getSaveGameManager() { return mSaveGameManager; }

AudioManager& EngineBravo::getAudioManager() { return mAudioManager; }

EventManager& EngineBravo::getEventManager() { return mEventManager; }

UIManager& EngineBravo::getUIManager() { return mUIManager; }

NetworkManager& EngineBravo::getNetworkManager() { return mNetworkManager; }

/**
 * @brief Start the behaviour scripts of all objects in the current scene. This function is called at the start of the
 * engine loop.
 */
void EngineBravo::startBehaviourScripts()
{
	Scene& currentScene = mSceneManager.getCurrentScene();

	auto gameObjects = currentScene.getGameObjects();
	for (auto& gameObject : gameObjects)
	{
		if (!gameObject.get().isActive())
		{
			continue;
		}
		if (gameObject.get().hasComponent<IBehaviourScript>())
		{
			for (auto behaviourScript : gameObject.get().getComponents<IBehaviourScript>())
			{
				if (behaviourScript.get().hasScriptStarted())
				{
					continue;
				}
				behaviourScript.get().onStart();
				behaviourScript.get().setScriptStarted(true);
			}
		}
	}
}

/**
 * @brief Run the behaviour scripts of all objects in the current scene. This function is called every frame of the
 * engine. Only calls the onUpdate if the object is active.
 */
void EngineBravo::runBehaviourScripts()
{
	Scene& currentScene = mSceneManager.getCurrentScene();

	auto gameObjects = currentScene.getGameObjects();
	for (auto& gameObject : gameObjects)
	{
		if (!gameObject.get().isActive())
		{
			continue;
		}
		if (gameObject.get().hasComponent<IBehaviourScript>())
		{
			for (auto behaviourScript : gameObject.get().getComponents<IBehaviourScript>())
			{
				behaviourScript.get().onUpdate();
			}
		}
	}
}

PhysicsManager& EngineBravo::getPhysicsManager() { return mPhysicsManager; }

UpdateQueue& EngineBravo::getUpdateQueue() { return mUpdateQueue; }
