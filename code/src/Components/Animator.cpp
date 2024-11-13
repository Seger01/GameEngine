#include "Components/Animator.h"

Animator::Animator() {}

Animator::~Animator() {}

std::unique_ptr<Component> Animator::clone() const { return std::make_unique<Animator>(*this); }
