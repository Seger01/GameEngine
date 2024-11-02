#include "EngineBravo.h"

#include <SDL.h>

#include "IBehaviourScript.h"
#include "Input.h"
#include "ParticleEmitter.h"
#include "Renderer.h"

EngineBravo::EngineBravo() {}

EngineBravo::~EngineBravo() {}

EngineBravo& EngineBravo::getInstance() {
    static EngineBravo instance;
    return instance;
}

void EngineBravo::initizalize() {
    this->mResourceManager.setRenderer(&mRenderSystem.getRenderer());

    startBehaviourScripts();

    Time::initialize();
    return;
}

void EngineBravo::run() {
    Input& input = Input::getInstance();

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        Time::update();
        static long startOfFrame = 0;
        startOfFrame = Time::ticks;
        // Event handling
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        std::cout << "Time since start: " << Time::ticks << std::endl;
        std::cout << "Delta time: " << Time::deltaTime << std::endl;

        input.update();

        runBehaviourScripts();

        mParticleSystem.update(mSceneManager.getCurrentScene());

        mRenderSystem.render(mSceneManager.getCurrentScene());

        if (Time::ticks - startOfFrame < mMinFrameTimeMs) {
            SDL_Delay(mMinFrameTimeMs - (Time::ticks - startOfFrame));
        }
    }
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

    std::cout << "Amount of registered gameobjects: " << currentScene->getGameObjects().size() << std::endl;

    if (currentScene) {
        for (auto& gameObject : currentScene->getGameObjects()) {
            for (auto behaviourScript : gameObject->getComponents<IBehaviourScript>()) {
                behaviourScript->onUpdate();
            }
        }
    }
}
