#include "EngineBravo.h"

#include <SDL.h>
#include <chrono>
#include <thread>

#include "IBehaviourScript.h"
#include "Input.h"
#include "ParticleEmitter.h"
#include "Renderer.h"

EngineBravo::EngineBravo() : mFrameRateLimit(60) {}

EngineBravo::~EngineBravo() {}

EngineBravo& EngineBravo::getInstance() {
    static EngineBravo instance;
    return instance;
}

void EngineBravo::initizalize() {
    this->mResourceManager.setRenderer(&mRenderSystem.getRenderer());

    if (mSceneManager.sceneChanged()) {
    }
    startBehaviourScripts();

    Time::initialize();

    return;
}

void EngineBravo::run() {
    Input& input = Input::getInstance();

    bool quit = false;
    SDL_Event e;

    mRenderSystem.getWindow().showWindow();

    while (!quit) {
        Time::update();
        // Event handling
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        if (mSceneManager.sceneChanged()) {
            startBehaviourScripts();
            mPhysicsManager.startPhysicsEngine(mSceneManager.getCurrentScene()->getGameObjects(), Vector2(0, 0));
        }
        input.update();

        runBehaviourScripts();

        mPhysicsManager.updatePhysicsEngine(mSceneManager.getCurrentScene());

        mParticleSystem.update(mSceneManager.getCurrentScene());

        mRenderSystem.render(mSceneManager.getCurrentScene());

        limitFrameRate(mFrameRateLimit);
    }
}

// function that limits frame rate by keeping track of the time it takes to render a frame and delaying the next frame
// if it renders too quickly
void EngineBravo::limitFrameRate(int aFrameRate) {
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
    if (delayTime > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(delayTime));
    }
    frameStart = std::chrono::high_resolution_clock::now();
}

SceneManager& EngineBravo::getSceneManager() { return mSceneManager; }
RenderSystem& EngineBravo::getRenderSystem() { return mRenderSystem; }
ResourceManager& EngineBravo::getResourceManager() { return mResourceManager; }

void EngineBravo::startBehaviourScripts() {
    Scene* currentScene = mSceneManager.getCurrentScene();
    if (currentScene == nullptr) {
        std::cout << "retrieved scene is nullptr" << std::endl;
    }

    if (currentScene) {
        for (auto& gameObject : currentScene->getGameObjects()) {
            for (auto behaviourScript : gameObject->getComponents<IBehaviourScript>()) {
                behaviourScript->onStart();
            }
        }
    }
}

void EngineBravo::runBehaviourScripts() {
    Scene* currentScene = mSceneManager.getCurrentScene();
    if (currentScene == nullptr) {
        std::cout << "retrieved scene is nullptr" << std::endl;
    }

    if (currentScene) {
        for (auto& gameObject : currentScene->getGameObjects()) {
            for (auto behaviourScript : gameObject->getComponents<IBehaviourScript>()) {
                behaviourScript->onUpdate();
            }
        }
    }
}

PhysicsManager& EngineBravo::getPhysicsManager() { return mPhysicsManager; }
