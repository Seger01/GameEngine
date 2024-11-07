#ifndef ENGINEBRAVO_H
#define ENGINEBRAVO_H

#include "AnimationManager.h"
#include "Network/NetworkManager.h"
#include "EventManager.h"
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

    void initialize();
    void run();

    void setFrameRateLimit(int aFrameRate);

    SceneManager& getSceneManager();
    RenderSystem& getRenderSystem();
    ResourceManager& getResourceManager();
    NetworkManager& getNetworkManager();
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

    void handleEvent(const Event& aEvent);

private:
    int mFrameRateLimit;
    bool mRunning;

    SceneManager mSceneManager;
    RenderSystem mRenderSystem;
    ResourceManager mResourceManager;
    ParticleSystem mParticleSystem;
    NetworkManager mNetworkManager;
    EventManager mEventManager;
    SaveGameManager saveGameManager;
    // AnimationManager animationManager;
    // UIManager uiManager;
};

#endif // ENGINEBRAVO_H
