#prama once

#include "AudioResourceManager.h"
#include "AudioSource.h"
#include "GameObject.h"
#include "IAudioFacade.h"
#include <vector>

class AudioManager {
public:
  void init();
  void play(const AudioSource &);
  void wake();
  IAudioFacade &getFacade();

private:
  std::vector<std::reference_wrapper<GameObject>> mGameObjects;
  AudioResourceManager mResources;
};