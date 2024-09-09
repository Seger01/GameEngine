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
    FMOD::Sound* sound = nullptr;
    result = system->createSound("../../resources/engine.mp3", FMOD_3D, 0, &sound); // Load your sound
    ERRCHECK(result);

    // Set the sound as 3D
    sound->set3DMinMaxDistance(1.0f, 1000.0f);

    // 4. Play the sound
    FMOD::Channel* channel = nullptr;
    result = system->playSound(sound, 0, false, &channel); // Play the sound immediately
    ERRCHECK(result);

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
    FMOD_VECTOR soundPos = {10.0f, 5.0f, 0.0f}; // 10 units in x direction
    FMOD_VECTOR soundVel = {5.0f, 0.0f, 0.0f}; // Not moving
    channel->set3DAttributes(&soundPos, &soundVel);

    // 5. Loop to keep FMOD updating and sound playing
    bool isPlaying = true;
    while (isPlaying) {
        system->update(); // Regular FMOD updates

        // Update location
        soundPos.x -= soundVel.x * 0.1f;
        channel->set3DAttributes(&soundPos, &soundVel);

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        // Check if the sound is still playing
        bool playing = false;
        channel->isPlaying(&playing);
        if (!playing) {
            isPlaying = false; // Stop the loop when the sound ends
        }
    }

    // 6. Clean up
    result = sound->release(); // Release the sound resource
    ERRCHECK(result);
    result = system->close();  // Close the FMOD system
    ERRCHECK(result);
    result = system->release(); // Release the FMOD system object
    ERRCHECK(result);

    return 0;
}
