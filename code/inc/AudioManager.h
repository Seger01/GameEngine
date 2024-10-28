#pragma once

#include "AudioResourceManager.h"
#include "AudioSource.h"
#include "GameObject.h"
#include "IAudioFacade.h"
#include <functional>
#include <vector>

class AudioManager {
public:
  void play(const AudioSource &);
  void stop(const AudioSource &);
  void wake();
  IAudioFacade &getFacade();

public:
  void addSound(const GameObject &aGameObjecte);
  void removeSound(const GameObject &aGameObject);

private:
  std::vector<std::reference_wrapper<GameObject>> mGameObjects;
  AudioResourceManager mResources;
};