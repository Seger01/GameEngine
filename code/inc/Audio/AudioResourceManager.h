#pragma once

#include "ComponentPathPair.h"
#include <string>
#include <vector>

class AudioSource;

class AudioResourceManager {
public:
  std::string getPath(const AudioSource &aSource);

private:
  std::vector<ComponentPathPair> mPaths;
};