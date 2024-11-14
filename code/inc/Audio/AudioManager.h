#pragma once

#include "AudioResourceManager.h"
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
    void addSound(const GameObject& aGameObjecte);
    void removeSound(const GameObject& aGameObject);

private:
    std::vector<std::reference_wrapper<const GameObject>> mGameObjects;
    AudioResourceManager mResources;
    std::unique_ptr<IAudioFacade> mFacade;
};