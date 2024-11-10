#include "AudioManager.h"
#include "AudioSource.h"
#include "IAudioFacade.h"
#include "MixerFacade.h"
#include <stdexcept>

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
        for (AudioSource* component : gameObject.get().getComponents<AudioSource>()) {
            if (component->getPlayOnWake()) {
                play(*component);
            }
        }
    }
}

IAudioFacade& AudioManager::getFacade() { return *mFacade.get(); }

void AudioManager::addSound(const GameObject& aGameObject) {
    mGameObjects.push_back(aGameObject);
    for (AudioSource* audioSource : aGameObject.getComponents<AudioSource>()) {
        if (audioSource->isMusic()) {
            mFacade->addMusic(audioSource->getFileName());
            continue;
        }
        mFacade->addSound(audioSource->getFileName());
    }

    mFacade->addSound(aGameObject.getComponents<AudioSource>().front()->getFileName());
}

void AudioManager::removeSound(const GameObject& aGameObject) {
    auto it = std::find_if(mGameObjects.begin(), mGameObjects.end(),
                           [&aGameObject](const GameObject& obj) { return &obj == &aGameObject; });
    if (it != mGameObjects.end()) {
        mGameObjects.erase(it);
    } else {
        throw std::logic_error("GameObject not found in AudioManager.");
    }
}