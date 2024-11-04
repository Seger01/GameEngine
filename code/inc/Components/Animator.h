#pragma once
#include "Component.h"

class Animator : public Component
{
    public:
        Animator();
        ~Animator();
        //void play(??? aLooping);
        void stop();
    private:
        //??? mFps;
};