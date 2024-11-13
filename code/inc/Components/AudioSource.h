#pragma once
#include <string>
#include "Component.h"

class AudioSource : public Component
{
    public:
        AudioSource();
        ~AudioSource();

        std::unique_ptr<Component> clone() const override;

        void stop();

    private:
        std::string mAudioClip;
        bool mPlayOnAwake;
        bool mLoop;
        //??? mVolume;
};