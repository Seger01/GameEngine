#include "AudioManager.h"
#include "EngineBravo.h"
#include "FSConverter.h"
#include "MixerContainer.h"
#include "MixerFacade.h"
#include <gtest/gtest.h>

class AudioTest : public ::testing::Test {
protected:
    EngineBravo* engine;
    AudioManager* audioManager;

    void SetUp() override {
        engine = &EngineBravo::getInstance();
        audioManager = &engine->getAudioManager();
    }
};

TEST_F(AudioTest, AddSound) {
    // Add a sound to the audio manager
    AudioSource audio("Audio/gun1.wav", false);

    // Verify that the sound is not loaded
    ASSERT_FALSE(audioManager->getFacade().audioIsLoaded("Audio/gun1.wav"));

    // Load the sound
    audioManager->loadSound(audio);

    // Verify that the sound was added
    ASSERT_TRUE(audioManager->getFacade().audioIsLoaded("Audio/gun1.wav"));
}

TEST_F(AudioTest, AddMusic) {
    // Add a music file to the audio manager
    AudioSource audio("Audio/music.wav", true);

    // Verify that the music is not loaded
    ASSERT_FALSE(audioManager->getFacade().musicIsLoaded());

    // Load the music
    audioManager->loadSound(audio);

    // Verify that the music was added
    ASSERT_TRUE(audioManager->getFacade().musicIsLoaded());
}

TEST_F(AudioTest, PlaySound) {
    // Add a sound to the audio manager
    std::string path = "Audio/gun1.wav";
    AudioSource audio(path, false);
    audioManager->loadSound(audio);

    // Play the sound
    audioManager->play(audio);

    // Verify that the sound is playing
    ASSERT_TRUE(audioManager->getFacade().isPlaying(path));
}

TEST_F(AudioTest, PlayMusic) {
    // Add a music file to the audio manager
    std::string path = "Audio/music.wav";
    AudioSource audio(path, true);
    audioManager->loadSound(audio);

    // Play the music
    audioManager->play(audio);

    // Verify that the music is playing
    ASSERT_TRUE(audioManager->getFacade().isMusicPlaying());
}

TEST_F(AudioTest, unloadSounds) {
    audioManager->clearSounds();
    // Add a sound to the audio manager
    std::string path1 = "Audio/gun1.wav";
    AudioSource audio1(path1, false);
    audioManager->loadSound(audio1);

    std::string path2 = "Audio/Steps_tiles-002.wav";
    AudioSource audio2(path2, false);
    audioManager->loadSound(audio2);

    // Unload all sounds
    audioManager->clearSounds();

    // Verify that the sound is not loaded
    ASSERT_FALSE(audioManager->getFacade().audioIsLoaded(path1));
    ASSERT_FALSE(audioManager->getFacade().audioIsLoaded(path2));
}

TEST_F(AudioTest, stopMusic) {
    // Add a music file to the audio manager
    std::string path = "Audio/music.wav";
    AudioSource audio(path, true);
    audioManager->loadSound(audio);

    // Play the music
    audioManager->play(audio);
    ASSERT_TRUE(audioManager->getFacade().isMusicPlaying());

    // Stop the music
    audioManager->stop(audio);

    // Verify that the music is not playing
    ASSERT_FALSE(audioManager->getFacade().isMusicPlaying());
}

TEST_F(AudioTest, stopSound) {
    // Add a sound to the audio manager
    std::string path = "Audio/gun1.wav";
    AudioSource audio(path, false);
    audioManager->loadSound(audio);

    // Play the sound
    audioManager->play(audio);
    ASSERT_TRUE(audioManager->getFacade().isPlaying(path));

    // Stop the sound
    ASSERT_THROW(audioManager->stop(audio), std::logic_error);
}

TEST_F(AudioTest, playUnloadedSound) {
    audioManager->clearSounds();
    // Add a sound to the audio manager
    std::string path = "Audio/gun1.wav";
    AudioSource audio(path, false);

    // Verify that the sound is not loaded
    ASSERT_FALSE(audioManager->getFacade().audioIsLoaded(path));

    // Play the sound
    audioManager->play(audio);

    // Verify that the sound is playing
    ASSERT_TRUE(audioManager->getFacade().isPlaying(path));
}

TEST_F(AudioTest, playUnloadedMusci) {
    audioManager->clearSounds();
    // Add a music file to the audio manager
    std::string path = "Audio/music.wav";
    AudioSource audio(path, true);

    // Verify that the music is not loaded
    ASSERT_FALSE(audioManager->getFacade().musicIsLoaded());

    // Play the music
    audioManager->play(audio);

    // Verify that the music is playing
    ASSERT_TRUE(audioManager->getFacade().isMusicPlaying());
}

TEST_F(AudioTest, playOnWake) {
    // create a scene
    engine->getSceneManager().createScene("testScene");
    GameObject* gameObject = new GameObject();
    engine->getSceneManager().requestSceneChange("testScene");
    engine->getSceneManager().getCurrentScene()->addGameObject(gameObject);

    // Add a sound to the audio manager
    std::string path = "Audio/gun1.wav";
    AudioSource* audio{new AudioSource(path, false)};
    audio->setPlayOnWake(true);
    gameObject->addComponent(audio);

    // Verify that the sound is not playing
    ASSERT_FALSE(audioManager->getFacade().isPlaying(path));

    // Wake the audio manager
    audioManager->wake();

    // Verify that the sound is playing
    ASSERT_TRUE(audioManager->getFacade().isPlaying(path));
}

TEST_F(AudioTest, containerLoadNonExistent) {
    MixerContainer container;
    ASSERT_EQ(container.getSound("nonexistent"), nullptr);
}

TEST_F(AudioTest, MixerFacadeLoadInvalid) {
    MixerFacade mixer;
    ASSERT_THROW(mixer.loadSound("nonexistent"), std::runtime_error);
}

TEST_F(AudioTest, MixerFacadeLoadInvalidMusic) {
    MixerFacade mixer;
    ASSERT_THROW(mixer.loadMusic("nonexistent"), std::runtime_error);
}

TEST_F(AudioTest, FloodMixerChannels) {
    std::string fullPath = FSConverter().getResourcePath("Audio/gun1.wav");
    // Test if, when playing a sound on each channel, the next sound is played on the last played channel
    MixerFacade mixer;
    for (int i = 0; i < MIX_CHANNELS; ++i) {
        mixer.loadSound(fullPath);
        mixer.playSound(fullPath, false, 100, 0);
    }
    mixer.loadSound(fullPath);
    ASSERT_EQ(mixer.findAvailableChannel(), 0);
    ASSERT_EQ(mixer.findAvailableChannel(), 1); // The channel should increment, because channel 0 is assumed used.
}

TEST_F(AudioTest, PlayComponent) {
    // Add a sound to the audio manager
    std::string path = "Audio/gun1.wav";
    AudioSource audio(path, false);

    // Create a game object and add the audio source component
    GameObject gameObject;
    AudioSource* audioComponent = new AudioSource(path, false);
    gameObject.addComponent(audioComponent);

    // Play the sound
    audioComponent->play(false);

    // Verify that the sound is playing
    ASSERT_TRUE(audioManager->getFacade().isPlaying(path));
}

TEST_F(AudioTest, StopComponent) {
    // Add a sound to the audio manager
    std::string path = "Audio/gun1.wav";
    AudioSource audio(path, false);

    // Create a game object and add the audio source component
    GameObject gameObject;
    AudioSource* audioComponent = new AudioSource(path, false);
    gameObject.addComponent(audioComponent);

    // Play the sound
    audioComponent->play(false);

    // Verify that the sound is playing
    ASSERT_TRUE(audioManager->getFacade().isPlaying(path));

    // Verify that the sound cannot be stopped
    ASSERT_THROW(audioComponent->stop(), std::logic_error);
}

TEST_F(AudioTest, componentVolume) {
    AudioSource audio("Audio/gun1.wav", false);
    audio.setVolume(50);
    ASSERT_EQ(audio.getVolume(), 50);
    audio.setVolume(0);
    ASSERT_EQ(audio.getVolume(), 0);
    audio.setVolume(100);
    ASSERT_EQ(audio.getVolume(), 100);
    audio.setVolume(101);
    ASSERT_EQ(audio.getVolume(), 100);
}

TEST_F(AudioTest, componentDirection) {
    AudioSource audio("Audio/gun1.wav", false);
    audio.setXDirection(-1);
    ASSERT_EQ(audio.getXDirection(), -1);
    audio.setXDirection(0);
    ASSERT_EQ(audio.getXDirection(), 0);
    audio.setXDirection(1);
    ASSERT_EQ(audio.getXDirection(), 1);
    audio.setXDirection(200);
    ASSERT_EQ(audio.getXDirection(), 100);
    audio.setXDirection(-200);
    ASSERT_EQ(audio.getXDirection(), -100);
}

TEST_F(AudioTest, componentClone) {
    AudioSource audio("Audio/gun1.wav", false);
    AudioSource* clone = dynamic_cast<AudioSource*>(audio.clone().get());
    ASSERT_EQ(clone->getFileName(), audio.getFileName());
    ASSERT_EQ(clone->getLooping(), audio.getLooping());
    ASSERT_EQ(clone->getPlayOnWake(), audio.getPlayOnWake());
    ASSERT_EQ(clone->getVolume(), audio.getVolume());
    ASSERT_EQ(clone->getXDirection(), audio.getXDirection());
}