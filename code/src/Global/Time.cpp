/**
 * @file Time.cpp
 * @brief Time class implementation file.
 */

#include "Time.h"

// Initialize static members
std::chrono::high_resolution_clock::time_point Time::startTime;
std::chrono::high_resolution_clock::time_point Time::lastFrameTime;
double Time::deltaTime = 0.0;
double Time::rawDeltaTime = 0.0;
double Time::ticks = 0.0;
double Time::timeDilation = 1.0;
