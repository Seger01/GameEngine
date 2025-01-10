#include "Time.h"
#include <gtest/gtest.h>
#include <thread>

// Test case for Time class initialization
TEST(TimeTest, Initialization)
{
	// Initialize Time class
	Time::initialize();

	// Check initial values
	EXPECT_DOUBLE_EQ(Time::deltaTime, 0.0);
	EXPECT_DOUBLE_EQ(Time::rawDeltaTime, 0.0);
	EXPECT_DOUBLE_EQ(Time::ticks, 0.0);
	EXPECT_DOUBLE_EQ(Time::timeDilation, 1.0);
}

// Test case for Time class update
TEST(TimeTest, Update)
{
	// Initialize Time class
	Time::initialize();

	// Wait for some time to simulate a frame
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	// Update the Time class
	Time::update();

	// Check rawDeltaTime is approximately 0.1 seconds
	EXPECT_NEAR(Time::rawDeltaTime, 0.1, 0.01);

	// Check deltaTime matches rawDeltaTime (default timeDilation is 1.0)
	EXPECT_DOUBLE_EQ(Time::deltaTime, Time::rawDeltaTime);

	// Check ticks are incremented correctly
	EXPECT_NEAR(Time::ticks, Time::deltaTime, 0.01);
}

// Test case for Time class with time dilation
TEST(TimeTest, TimeDilation)
{
	// Initialize Time class
	Time::initialize();

	// Set time dilation factor
	Time::timeDilation = 0.5; // Half-speed

	// Wait for some time to simulate a frame
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	// Update the Time class
	Time::update();

	// Check deltaTime is half of rawDeltaTime
	EXPECT_NEAR(Time::deltaTime, Time::rawDeltaTime * 0.5, 0.01);

	// Check ticks are incremented correctly
	EXPECT_NEAR(Time::ticks, Time::deltaTime, 0.01);
}

// Test case for resetting time dilation to zero
TEST(TimeTest, TimeDilationZero)
{
	// Initialize Time class
	Time::initialize();

	// Set time dilation factor to zero
	Time::timeDilation = 0.0;

	// Wait for some time to simulate a frame
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	// Update the Time class
	Time::update();

	// Check deltaTime is zero
	EXPECT_DOUBLE_EQ(Time::deltaTime, 0.0);

	// Check rawDeltaTime is unaffected
	EXPECT_NEAR(Time::rawDeltaTime, 0.1, 0.01);

	// Check ticks are not incremented
	EXPECT_DOUBLE_EQ(Time::ticks, 0.0);
}
