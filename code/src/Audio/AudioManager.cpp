#include "AudioManager.h"
#include "MixerFacade.h"
#include <exception>

AudioManager::AudioManager() { mFacade = std::make_unique<MixerFacade>(); }

void AudioManager::play(const AudioSource& aSource) {
    if (aSource.isMusic()) {
        getFacade().playMusic();
    } else {
        getFacade().playSound(aSource.getFileName(), aSource.getLooping(), aSource.getVolume());
    }
}

void AudioManager::stop(const AudioSource& aSource) {
    if (aSource.isMusic()) {
        getFacade().stopMusic();
    } else {
        throw std::logic_error("Only music sources can be stopped.");
    }
}

void AudioManager::wake() {
    for (auto& gameObject : mGameObjects) {
        if (gameObject.get().getComponent<AudioSource>().getPlayOnWake()) {
            play(gameObject.get().getComponent<AudioSource>());
        }
    }
}

IAudioFacade& AudioManager::getFacade() { return *mFacade.get(); }

void AudioManager::addSound(const GameObject& aGameObject) { mGameObjects.push_back(aGameObject); }

void AudioManager::removeSound(const GameObject& aGameObject) {
    auto it = std::find_if(mGameObjects.begin(), mGameObjects.end(),
                           [&aGameObject](const GameObject& obj) { return &obj == &aGameObject; });
    if (it != mGameObjects.end()) {
        mGameObjects.erase(it);
    } else {
        throw std::logic_error("GameObject not found in AudioManager.");
    }
}