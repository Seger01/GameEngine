#include "AudioManager.h"
#include "AudioSource.h"
#include "EngineBravo.h"
#include "IAudioFacade.h"
#include "MixerFacade.h"
#include "Scene.h"
#include <stdexcept>

/**
 * @brief Construct a new AudioManager::AudioManager object. Currently creates a MixerFacade object.
 */
AudioManager::AudioManager() { mFacade = std::make_unique<MixerFacade>(); }

/**
 * @brief Play the audio source.
 *
 * @param aSource The audio source to play. Uses the file name, looping, volume, and x direction.
 */
void AudioManager::play(const AudioSource& aSource) {
    if (aSource.isMusic()) {
        // If the music is not loaded, load it
        if (!getFacade().musicIsLoaded()) {
            getFacade().loadMusic(aSource.getFileName());
        }
        getFacade().playMusic(aSource.getVolume());
    } else {
        // If the sound is not loaded, load it
        if (!getFacade().audioIsLoaded(aSource.getFileName())) {
            getFacade().loadMusic(aSource.getFileName());
        }
        getFacade().playSound(aSource.getFileName(), aSource.getLooping(), aSource.getVolume(),
                              aSource.getXDirection());
    }
}

/**
 * @brief Stop the audio source. If it is not a music source, throw an error.
 */
void AudioManager::stop(const AudioSource& aSource) {
    if (aSource.isMusic()) {
        getFacade().stopMusic();
    } else {
        throw std::logic_error("Only music sources can be stopped.");
    }
}

/**
 * @brief Plays all the audio sources in the current scene, where play on wake is true
 */
void AudioManager::wake() {
    // Get the current scene
    EngineBravo& engine = EngineBravo::getInstance();
    Scene* currScene = engine.getSceneManager().getCurrentScene();

    // Iterate through all objects in the scene
    for (auto& gameObject : currScene->getGameObjects()) {
        // Iterate through all audio sources in the object
        for (AudioSource* component : gameObject->getComponents<AudioSource>()) {
            if (component->getPlayOnWake()) {
                play(*component);
            }
        }
    }
}

IAudioFacade& AudioManager::getFacade() { return *mFacade.get(); }

/**
 * @brief Load the sound from the audio source into memory
 */
void AudioManager::loadSound(const AudioSource& aAudio) {
    if (aAudio.isMusic()) {
        getFacade().loadMusic(aAudio.getFileName());
    } else {
        getFacade().loadSound(aAudio.getFileName());
    }
}

void AudioManager::clearSounds() { getFacade().unloadAll(); }