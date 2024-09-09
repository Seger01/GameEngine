#include <fmod.hpp>
#include <fmod_errors.h>
#include <iostream>

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
    result = system->createSound("../../resources/effect1.mp3", FMOD_DEFAULT, 0, &sound); // Load your sound
    ERRCHECK(result);

    // 4. Play the sound
    FMOD::Channel* channel = nullptr;
    result = system->playSound(sound, 0, false, &channel); // Play the sound immediately
    ERRCHECK(result);

    // 5. Loop to keep FMOD updating and sound playing
    bool isPlaying = true;
    while (isPlaying) {
        system->update(); // Regular FMOD updates

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
