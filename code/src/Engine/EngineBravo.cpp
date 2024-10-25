#include "EngineBravo.h"

#include "IBehaviourScript.h"
#include "Renderer.h"
#include "SDL.h"

EngineBravo::EngineBravo() {}

EngineBravo::~EngineBravo() {}

void EngineBravo::initizalize() {
    this->mResourceManager.setRenderer(&mRenderSystem.getRenderer());

    return;
}

void EngineBravo::run() {
    bool quit = false;
    SDL_Event e;

    while (!quit) {
        // Event handling
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        runBehaviourScripts();

        mRenderSystem.render(mSceneManager.getCurrentScene());

        SDL_Delay(200);
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
            std::cout << "looping through behaviour scritps" << std::endl;
            if (gameObject->getComponent<IBehaviourScript>() != nullptr) {
                gameObject->getComponent<IBehaviourScript>()->onUpdate();
            }
        }
    }
}
