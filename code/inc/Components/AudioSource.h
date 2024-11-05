#pragma once
#include <string>
#include "Component.h"

class AudioSource : public Component
{
    public:
        AudioSource();
        ~AudioSource();
        //void play(??? aLooping);
        void stop();
    private:
        std::string mAudioClip;
        bool mPlayOnAwake;
        bool mLoop;
        //??? mVolume;
};