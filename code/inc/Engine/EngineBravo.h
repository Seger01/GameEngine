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
    void runBehaviourScripts();

    SceneManager& getSceneManager();
    RenderSystem& getRenderSystem();
    ResourceManager& getResourceManager();

private:
    SceneManager mSceneManager;
    RenderSystem mRenderSystem;
    ResourceManager mResourceManager;
    // SaveGameManager saveGameManager;
    // AnimationManager animationManager;
    // UIManager uiManager;
};
