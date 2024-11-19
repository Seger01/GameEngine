#include "AudioManager.h"
#include "EngineBravo.h"
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
    AudioSource audio("gun1.wav", false);

    // Verify that the sound is not loaded
    ASSERT_FALSE(audioManager->getFacade().audioIsLoaded("gun1.wav"));

    // Load the sound
    audioManager->loadSound(audio);

    // Verify that the sound was added
    ASSERT_TRUE(audioManager->getFacade().audioIsLoaded("test.wav"));
}