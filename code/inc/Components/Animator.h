#pragma once
#include "Component.h"

class Animator : public Component
{
    public:
        Animator();
        ~Animator();

        std::unique_ptr<Component> clone() const override;

        void stop();
    private:
        //??? mFps;
};