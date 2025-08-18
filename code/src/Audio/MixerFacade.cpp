#include "Audio/MixerFacade.h"
#include <soloud_wav.h>
#include <soloud_wavstream.h>
#include <stdexcept>

MixerFacade::MixerFacade() : mMusicHandle(0) { mEngine.init(); }

MixerFacade::~MixerFacade()
{
	mEngine.deinit();
	unloadAll();
}

void MixerFacade::loadSound(const std::string& aPath)
{
	if (!audioIsLoaded(aPath))
	{
		auto* wav = new SoLoud::Wav();
		if (wav->load(aPath.c_str()) != SoLoud::SO_NO_ERROR)
		{
			delete wav;
			throw std::runtime_error("Failed to load sound: " + aPath);
		}
		mMixerContainer.addSound(aPath, wav);
	}
}

void MixerFacade::loadMusic(const std::string& aPath)
{
	if (!musicIsLoaded())
	{
		auto* stream = new SoLoud::WavStream();
		if (stream->load(aPath.c_str()) != SoLoud::SO_NO_ERROR)
		{
			delete stream;
			throw std::runtime_error("Failed to load music: " + aPath);
		}
		mMixerContainer.addMusic(aPath, stream);
	}
}

void MixerFacade::unloadAll()
{
	mEngine.stopAll();
	mMixerContainer.clear();
	mSoundHandles.clear();
	mMusicHandle = 0;
}

bool MixerFacade::audioIsLoaded(const std::string& aPath) const { return mMixerContainer.getSound(aPath) != nullptr; }

bool MixerFacade::musicIsLoaded() const { return mMixerContainer.getMusic() != nullptr; }

void MixerFacade::playSound(const std::string& aPath, bool aLooping, unsigned aVolume, int aDirection)
{
	SoLoud::Wav* wav = mMixerContainer.getSound(aPath);
	if (!wav)
		return;
	SoLoud::handle handle = mEngine.play(*wav, aVolume / 100.0f, 0.0f, 0.0f, aLooping ? 1 : 0);
	mSoundHandles[aPath] = handle;
}

void MixerFacade::playMusic(int aVolume)
{
	SoLoud::WavStream* music = mMixerContainer.getMusic();
	if (!music)
		return;
	mMusicHandle = mEngine.play(*music, aVolume / 100.0f);
}

void MixerFacade::pauseMusic()
{
	if (mMusicHandle != 0)
		mEngine.setPause(mMusicHandle, true);
}

void MixerFacade::resumeMusic()
{
	if (mMusicHandle != 0)
		mEngine.setPause(mMusicHandle, false);
}

void MixerFacade::stopMusic()
{
	if (mMusicHandle != 0)
	{
		mEngine.stop(mMusicHandle);
		mMusicHandle = 0;
	}
}

bool MixerFacade::isPlaying(const std::string& aPath)
{
	auto it = mSoundHandles.find(aPath);
	if (it != mSoundHandles.end())
		return mEngine.getPause(it->second) == 0 && mEngine.isValidVoiceHandle(it->second);
	return false;
}

bool MixerFacade::isMusicPlaying()
{
	return mMusicHandle != 0 && mEngine.getPause(mMusicHandle) == 0 && mEngine.isValidVoiceHandle(mMusicHandle);
}

int MixerFacade::distanceToAngle(int aDirection) const
{
	// Stub: implement spatialization as needed for your game
	return aDirection;
}
