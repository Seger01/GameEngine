#pragma once

#include "IAudioFacade.h"
#include <memory>
#include <vector>

class AudioSource;

class AudioManager {
public:
	AudioManager();

public:
	void play(const AudioSource& aAudio);
	void stop(const AudioSource& aAudio);
	void wake();

public:
	IAudioFacade& getFacade();

public:
	void loadSound(const AudioSource& aAudio);
	void clearSounds();

private:
	std::unique_ptr<IAudioFacade> mFacade;
};