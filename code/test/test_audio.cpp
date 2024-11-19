#include "AudioManager.h"
#include "EngineBravo.h"
#include "MixerContainer.h"
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