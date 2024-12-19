/**
 * @file Time.h
 * @brief Time class for handling the ticks and the delta time of the game.
 */

#ifndef TIME_H
#define TIME_H

#include <chrono>

/**
 * @class Time
 * @brief Class for handling the ticks and the delta time of the game.
 */
class Time
{
public:
	// Call this once at the start of the application
	static void initialize()
	{
		startTime = std::chrono::high_resolution_clock::now();
		lastFrameTime = startTime;
		ticks = 0.0;
		deltaTime = 0.0;
		timeDilation = 1.0; // Default time dilation factor is 1 (normal speed)
	}

	// Call this once per frame to update deltaTime and elapsed time since start
	static void update()
	{
		auto currentTime = std::chrono::high_resolution_clock::now();

		// Calculate raw deltaTime in seconds
		rawDeltaTime = std::chrono::duration<double>(currentTime - lastFrameTime).count();

		if (timeDilation <= 0.0)
		{
			timeDilation = 0.0;
		}
		// Apply time dilation
		deltaTime = rawDeltaTime * timeDilation;

		// Update lastFrameTime to the current time
		lastFrameTime = currentTime;

		// Accumulate total elapsed time in seconds
		ticks += deltaTime;
	}

	static double deltaTime;	// Time in seconds since last frame, affected by time dilation
	static double rawDeltaTime; // Time in seconds since last frame, not affected by time dilation
	static double ticks;		// Total time since start in seconds, affected by time dilation
	static double timeDilation; // Time dilation factor (e.g., 0.5 for half speed, 2.0 for double speed)

private:
	static std::chrono::high_resolution_clock::time_point startTime;
	static std::chrono::high_resolution_clock::time_point lastFrameTime;
};

#endif // TIME_H
