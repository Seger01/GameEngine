#include <fmod.hpp>
#include <fmod_errors.h>
#include <iostream>
#include <chrono>
#include <thread>

void ERRCHECK(FMOD_RESULT result) {
    if (result != FMOD_OK) {
        std::cerr << "FMOD error! (" << result << ") " << FMOD_ErrorString(result) << std::endl;
        exit(-1);
    }
}

int main() {
    // 1. Create an FMOD system object
    FMOD::System* system = nullptr;
    FMOD_RESULT result = FMOD::System_Create(&system); // Create the main system object.
    ERRCHECK(result);

    // 2. Initialize the FMOD system
    result = system->init(512, FMOD_INIT_NORMAL, 0); // Initialize FMOD system
    ERRCHECK(result);

    // 3. Load a sound file (sound will be loaded into memory)
    FMOD::Sound* soundC = nullptr;
    result = system->createSound("../../resources/guitar_C3_very-long_forte_normal.mp3", FMOD_3D, 0, &soundC); // Load your sound
    ERRCHECK(result);
    FMOD::Sound* soundE = nullptr;
    result = system->createSound("../../resources/guitar_E3_very-long_forte_normal.mp3", FMOD_3D, 0, &soundE); // Load your sound
    ERRCHECK(result);
    FMOD::Sound* soundG = nullptr;
    result = system->createSound("../../resources/guitar_G3_very-long_forte_normal.mp3", FMOD_3D, 0, &soundG); // Load your sound
    ERRCHECK(result);

    // Set the sound as 3D
    soundC->set3DMinMaxDistance(1.0f, 1000.0f);
    soundE->set3DMinMaxDistance(1.0f, 1000.0f);
    soundG->set3DMinMaxDistance(1.0f, 1000.0f);

    // 4. Play the sound
    FMOD::Channel* channelL = nullptr;
    FMOD::Channel* channelR = nullptr;
    result = system->playSound(soundC, 0, false, &channelL); // Play the sound immediately
    ERRCHECK(result);
    result = system->playSound(soundE, 0, false, &channelR); // Play the sound immediately
    ERRCHECK(result);
    result = system->playSound(soundG, 0, false, &channelR); // Play the sound immediately
    ERRCHECK(result);
    channelL->setVolume(4.5);
    channelR->setVolume(4.5);

    // Listener position, facing forward
    FMOD_VECTOR listenerPos = {0.0f, 0.0f, 0.0f};
    FMOD_VECTOR listenerVel = {0.0f, 0.0f, 0.0f};
    FMOD_VECTOR listenerForward = {0.0f, 0.0f, 1.0f};
    FMOD_VECTOR listenerUp = {0.0f, 1.0f, 0.0f};
    system->set3DListenerAttributes(0, &listenerPos, &listenerVel, &listenerForward, &listenerUp);

    // Set Doppler effect
    float dopplerScale = 1.0f;    // Default Doppler effect scale
    float distanceFactor = 1.0f;  // World units per meter
    float rolloffScale = 1.0f;    // Default rolloff scale
    system->set3DSettings(dopplerScale, distanceFactor, rolloffScale);

    // Sound source position
    FMOD_VECTOR soundLPos = {-3.0f, 0.0f, 0.0f}; // 10 units in x direction
    FMOD_VECTOR soundLVel = {0.0f, 0.0f, 0.0f}; // Not moving
    channelL->set3DAttributes(&soundLPos, &soundLVel);
    FMOD_VECTOR soundRPos = {3.0f, 0.0f, 0.0f}; // 10 units in x direction
    FMOD_VECTOR soundRVel = {0.0f, 0.0f, 0.0f}; // Not moving
    channelR->set3DAttributes(&soundRPos, &soundRVel);

    // 5. Loop to keep FMOD updating and sound playing
    bool isPlaying = true;
    while (isPlaying) {
        system->update(); // Regular FMOD updates

        // Update location
        soundLPos.x -= soundLVel.x * 0.1f;
        soundRPos.x -= soundRVel.x * 0.1f;
        channelL->set3DAttributes(&soundLPos, &soundLVel);
        channelR->set3DAttributes(&soundRPos, &soundRVel);

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        // Check if the sound is still playing
        bool playing = false;
        channelL->isPlaying(&playing);
        channelR->isPlaying(&playing);
        if (!playing) {
            isPlaying = false; // Stop the loop when the sound ends
        }
    }

    // 6. Clean up
    result = soundC->release(); // Release the sound resource
    ERRCHECK(result);
    result = soundE->release(); // Release the sound resource
    ERRCHECK(result);
    result = soundG->release(); // Release the sound resource
    ERRCHECK(result);
    result = system->close();  // Close the FMOD system
    ERRCHECK(result);
    result = system->release(); // Release the FMOD system object
    ERRCHECK(result);

    return 0;
}
