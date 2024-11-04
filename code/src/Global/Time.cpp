#include "Time.h"

std::chrono::high_resolution_clock::time_point Time::startTime;
std::chrono::milliseconds Time::lastFrameTime = std::chrono::milliseconds(0);
long Time::deltaTime = 0;
long Time::ticks = 0;
