#pragma once

#include <string>

class AudioSource;

struct ComponentPathPair {
  const AudioSource &mSource;
  const std::string mPath;
};