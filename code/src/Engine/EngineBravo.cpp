#include "EngineBravo.h"

#include "SDL.h"

EngineBravo::EngineBravo() {}

EngineBravo::~EngineBravo() {}

void EngineBravo::initizalize() {
}

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
        renderer.render(sceneManager.getCurrentScene());
        SDL_Delay(200);
    }
}

SceneManager& EngineBravo::getSceneManager() { return sceneManager; }

void EngineBravo::runBehaviourScripts() {
    // Scene* currentScene = sceneManager.getCurrentScene();
    // if (currentScene) {
    //     for (auto& gameObject : currentScene->getGameObjects()) {
    //         for (auto& script : gameObject->getComponent<SampleBehaviourScript>()) {
    //             script->onUpdate();
    //         }
    //     }
    // }
}
