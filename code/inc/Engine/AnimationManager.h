#pragma once
#include "Scene.h"
class AnimationManager
{   
    public: 
        friend class EngineBravo;
        void updateAnimations(Scene &scene);
    private:
        AnimationManager();
        ~AnimationManager();
};