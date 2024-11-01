#ifndef ENGINEBRAVO_H
#define ENGINEBRAVO_H

#include "AnimationManager.h"
#include "RenderSystem.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "SaveGameManager.h"
#include "SceneManager.h"
#include "Time.h"
#include "UIManager.h"

class EngineBravo {
public:
    // Get the single instance of EngineBravo
    static EngineBravo& getInstance();

    void initizalize();
    void run();

    SceneManager& getSceneManager();
    RenderSystem& getRenderSystem();
    ResourceManager& getResourceManager();

private:
    // Private constructor and destructor
    EngineBravo();
    ~EngineBravo();

    // Deleted copy constructor and assignment operator to prevent copying
    EngineBravo(const EngineBravo&) = delete;
    EngineBravo& operator=(const EngineBravo&) = delete;

    void runBehaviourScripts();
    void startBehaviourScripts();

private:
    SceneManager mSceneManager;
    RenderSystem mRenderSystem;
    ResourceManager mResourceManager;
    // SaveGameManager saveGameManager;
    // AnimationManager animationManager;
    // UIManager uiManager;
};

#endif // ENGINEBRAVO_H
