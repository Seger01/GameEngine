#pragma once

#include "AudioSource.h"
#include "IAudioFacade.h"
#include <memory>
#include <vector>

class GameObject;

class AudioManager
{
public:
	AudioManager();

public:
	void play(const AudioSource& aSource);
	void pause(const AudioSource& aSource);
	void resume(const AudioSource& aSource);
	void stop(const AudioSource& aSource);
	void wake();

public:
	IAudioFacade& getFacade();

public:
	void loadSound(const AudioSource& aAudio);
	void clearSounds();
	void addObject(GameObject& aObject);
	void removeObject(GameObject& aObject);
	const std::vector<std::reference_wrapper<GameObject>>& getObjects() const;
	void clearObjects();

private:
	std::vector<std::reference_wrapper<GameObject>> mObjects;
	std::unique_ptr<IAudioFacade> mFacade;
};