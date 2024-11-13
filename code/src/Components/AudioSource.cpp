#include "Components/AudioSource.h"

AudioSource::AudioSource() {}

AudioSource::~AudioSource() {}

std::unique_ptr<Component> AudioSource::clone() const { return std::make_unique<AudioSource>(*this); }