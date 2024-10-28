#pragma once

#include <string>

class AudioSource;

class AudioResourceManager {
public:
  std::string getPath(const AudioSource &aSource);

private:
};