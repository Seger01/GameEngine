#include "Vector2.h" // Assuming Vector2 is defined elsewhere
#include "Window.h"
#include <SDL.h>
#include <gtest/gtest.h>

class WindowTest : public ::testing::Test
{
protected:
	Window* window;

	void SetUp() override
	{
		// Create a Window instance for testing
		window = new Window(800, 600);
	}

	void TearDown() override
	{
		// Clean up the Window instance
		delete window;
	}
};

TEST_F(WindowTest, Constructor_ValidInitialization)
{
	// Ensure the window was initialized correctly
	ASSERT_NE(window->getSDLWindow(), nullptr);
}

TEST_F(WindowTest, GetSize_ReturnsCorrectSize)
{
	// Verify the initial size of the window
	Vector2 size = window->getSize();
	EXPECT_EQ(size.x, 800);
	EXPECT_EQ(size.y, 600);
}

TEST_F(WindowTest, SetSize_UpdatesWindowSize)
{
	// Set a new window size
	Vector2 newSize(1024, 768);
	window->setSize(newSize);

	// Verify the updated size
	Vector2 updatedSize = window->getSize();
	EXPECT_EQ(updatedSize.x, 1024);
	EXPECT_EQ(updatedSize.y, 768);
}

TEST_F(WindowTest, ShowWindow)
{
	// Call showWindow and ensure no errors
	EXPECT_NO_THROW(window->showWindow());
}

TEST_F(WindowTest, SetFullScreen)
{
	// Set fullscreen mode and ensure no errors
	EXPECT_NO_THROW(window->setFullScreen());

	// Verify the window is in fullscreen mode
	Uint32 flags = SDL_GetWindowFlags(window->getSDLWindow());
	EXPECT_TRUE(flags & SDL_WINDOW_FULLSCREEN);
}

TEST_F(WindowTest, SetFloating)
{
	// Set floating (borderless) mode
	window->setFloating();

	// Verify the window is borderless
	Uint32 flags = SDL_GetWindowFlags(window->getSDLWindow());
	EXPECT_TRUE(flags & SDL_WINDOW_BORDERLESS);
}

TEST_F(WindowTest, ToggleFullFloating)
{
	// Initially set to fullscreen
	window->setFullScreen();

	// Toggle to floating mode
	window->toggleFullFloating();
	Uint32 flags = SDL_GetWindowFlags(window->getSDLWindow());

	// Toggle back to fullscreen mode
	window->toggleFullFloating();
	flags = SDL_GetWindowFlags(window->getSDLWindow());
	EXPECT_TRUE(flags & SDL_WINDOW_FULLSCREEN);
}

TEST(WindowStandaloneTest, Destructor_CleansUpResources)
{
	// Ensure the window is properly destroyed
	Window* tempWindow = new Window(800, 600);
	SDL_Window* sdlWindow = tempWindow->getSDLWindow();
	delete tempWindow;

	// Verify the SDL window is destroyed
	EXPECT_EQ(SDL_GetWindowFlags(sdlWindow), 0);
}
