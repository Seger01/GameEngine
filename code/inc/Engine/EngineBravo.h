#ifndef ENGINEBRAVO_H
#define ENGINEBRAVO_H

#include "AnimationManager.h"
#include "ParticleSystem.h"
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
    SaveGameManager& getSaveGameManager();

private:
    // Private constructor and destructor
    EngineBravo();
    ~EngineBravo();

    // Deleted copy constructor and assignment operator to prevent copying
    EngineBravo(const EngineBravo&) = delete;
    EngineBravo& operator=(const EngineBravo&) = delete;

    void runBehaviourScripts();
    void startBehaviourScripts();

    void limitFrameRate(int aFrameRate);

private:
    int mFrameRateLimit;

    SceneManager mSceneManager;
    RenderSystem mRenderSystem;
    ResourceManager mResourceManager;
    ParticleSystem mParticleSystem;
    SaveGameManager saveGameManager;
    // AnimationManager animationManager;
    // UIManager uiManager;
};

#endif // ENGINEBRAVO_H
