#pragma once

#include "AnimationManager.h"
#include "RenderSystem.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "SaveGameManager.h"
#include "SceneManager.h"
#include "UIManager.h"

class EngineBravo {
public:
    EngineBravo();
    ~EngineBravo();
    void initizalize();
    void run();
    SceneManager& getSceneManager();
    void runBehaviourScripts();

    RenderSystem& getRenderSystem();

private:
    SceneManager mSceneManager;
    RenderSystem mRenderSystem;
    // SaveGameManager saveGameManager;
    // ResourceManager resourceManager;
    // AnimationManager animationManager;
    // UIManager uiManager;
};
