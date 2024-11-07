#pragma once

#include <SDL_mixer.h>
#include <string>

class MixerContainer {
public:
    void addSound(std::string aPath, Mix_Chunk aSound);
    Mix_Chunk* getSound(std::string aIndex);
    void addMusic(Mix_Music aMusic);
    Mix_Music* getMusic();
    void clear();
};