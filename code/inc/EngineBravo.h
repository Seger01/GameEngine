#pragma once

#include "SceneManager.h"
#include "SaveGameManager.h"
#include "ResourceManager.h"
#include "AnimationManager.h"
#include "UIManager.h"

class EngineBravo
{
    public: 
        EngineBravo();
        ~EngineBravo();
        void initizalize();
        SceneManager& getSceneManager();
        void runBehaviourScripts();
    private: 
        SceneManager sceneManager;
        SaveGameManager saveGameManager;
        ResourceManager resourceManager;
        AnimationManager animationManager;
        UIManager uiManager;
};      
