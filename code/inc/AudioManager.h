#prama once

#include "GameObject.h"
#include <vector>

class AudioManager {
private:
  std::vector<std::reference_wrapper<GameObject>> mGameObjectsWAudio;
};