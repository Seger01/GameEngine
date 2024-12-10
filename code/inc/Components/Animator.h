#pragma once
#include "Component.h"

class Animator : public Component
{
    public:
        Animator(std::string aTag = "defaultAnimator");
        ~Animator();

        std::unique_ptr<Component> clone() const override;

        void stop();
    private:
        //??? mFps;
};