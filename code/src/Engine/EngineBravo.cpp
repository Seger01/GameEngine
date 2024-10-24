#include "EngineBravo.h"

#include "IBehaviourScript.h"
#include "Renderer.h"
#include "SDL.h"

EngineBravo::EngineBravo() {}

EngineBravo::~EngineBravo() {}

void EngineBravo::initizalize() {}

void EngineBravo::run() {
    // Scene* currentScene = sceneManager.getCurrentScene();
    // if (currentScene) {
    //     for (auto& gameObject : currentScene->getGameObjects()) {
    //         for (auto& script : gameObject->getComponent<SampleBehaviourScript>()) {
    //             script->onStart();
    //         }
    //     }
    // }
    while (true) {
        runBehaviourScripts();

        mRenderSystem.render(mSceneManager.getCurrentScene());
        // renderer.render(mSceneManager.getCurrentScene());
        SDL_Delay(200);
    }
}

SceneManager& EngineBravo::getSceneManager() { return mSceneManager; }

RenderSystem& EngineBravo::getRenderSystem() { return mRenderSystem; }

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
