#pragma once

#include "AudioSource.h"
#include "GameObject.h"
#include "IAudioFacade.h"
#include <functional>
#include <memory>
#include <vector>

class AudioManager {
public:
    AudioManager();
    void play(const AudioSource&);
    void stop(const AudioSource&);
    void wake();
    IAudioFacade& getFacade();

public:
    void addObject(const GameObject& aGameObjecte);
    void removeObject(const GameObject& aGameObject);

private:
    std::vector<std::reference_wrapper<const GameObject>> mGameObjects;
    std::unique_ptr<IAudioFacade> mFacade;
};