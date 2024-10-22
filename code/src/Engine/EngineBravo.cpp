#include "EngineBravo.h"

#include "SDL.h"

EngineBravo::EngineBravo() {}

EngineBravo::~EngineBravo() {}

void EngineBravo::initizalize() {}

void EngineBravo::run() {
    while (true) {
        runBehaviourScripts();
        SDL_Delay(200);
    }
}

SceneManager& EngineBravo::getSceneManager() { return sceneManager; }

void EngineBravo::runBehaviourScripts() {}
