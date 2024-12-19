/**
 * @file MixerContainer.h
 *
 * @brief This file contains the declaration of the MixerContainer class
 */
#pragma once

#include <SDL_mixer.h>
#include <string>
#include <unordered_map>

/**
 * @class MixerContainer
 *
 * @brief This class is responsible for owning SDL mixer sound effects and music. It ensures that every unique sound
 * effect is loaded into memory only once.
 */
class MixerContainer
{
public:
	MixerContainer();

public:												  // rule of five
	virtual ~MixerContainer();						  // Destructor
	MixerContainer(const MixerContainer&);			  // Copy constructor
	MixerContainer(MixerContainer&&);				  // Move constructor
	MixerContainer& operator=(const MixerContainer&); // Copy assignment operator
	MixerContainer& operator=(MixerContainer&&);	  // Move assignment operator

public:
	void addSound(const std::string& aPath, Mix_Chunk* aSound);
	Mix_Chunk* getSound(const std::string& aIndex);
	const Mix_Chunk* getSound(const std::string& aIndex) const;

	void addMusic(const std::string& aPath, Mix_Music* aMusic);
	Mix_Music* getMusic();
	const Mix_Music* getMusic() const;

	void clear();

private:
	/// @brief Map of sound effects. key is the path to the sound
	std::unordered_map<std::string, Mix_Chunk*> mSfx;
	/// @brief Path to the music file
	std::string mMusicPath;
	/// @brief The music file
	Mix_Music* mMusic;
};