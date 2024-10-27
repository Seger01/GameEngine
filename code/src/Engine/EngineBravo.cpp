#include "EngineBravo.h"

#include "IBehaviourScript.h"
#include "Renderer.h"
#include "SDL.h"

#include "Input.h"

EngineBravo::EngineBravo() {}

EngineBravo::~EngineBravo() {}

void EngineBravo::initizalize() {
    this->mResourceManager.setRenderer(&mRenderSystem.getRenderer());

    return;
}

void EngineBravo::run() {
    Input& input = Input::getInstance();

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        // Event handling
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        input.update();

        runBehaviourScripts();

        mRenderSystem.render(mSceneManager.getCurrentScene());

        SDL_Delay(16);
    }
}

SceneManager& EngineBravo::getSceneManager() { return mSceneManager; }
RenderSystem& EngineBravo::getRenderSystem() { return mRenderSystem; }
ResourceManager& EngineBravo::getResourceManager() { return mResourceManager; }

void EngineBravo::runBehaviourScripts() {
    Scene* currentScene = mSceneManager.getCurrentScene();
    if (currentScene == nullptr) {
        std::cout << "retrieved scene is nullptr" << std::endl;
    }

    std::cout << "Amount of registered gameobjects: " << currentScene->getGameObjects().size() << std::endl;

    if (currentScene) {
        for (auto& gameObject : currentScene->getGameObjects()) {
            // for (auto& script : gameObject->getComponent<IBehaviourScript>()) {
            //     script->onUpdate();
            // }
            if (gameObject->getComponent<IBehaviourScript>() != nullptr) {
                gameObject->getComponent<IBehaviourScript>()->onUpdate();
            }
        }
    }
}
