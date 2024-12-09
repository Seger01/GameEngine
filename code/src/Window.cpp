#include "Window.h"
#include "SDL_video.h"
#include <stdexcept>

/**
 * @brief Constructor for the Window class. Initializes the window with a specified width and height.
 * @param aWindowWidth The width of the window.
 * @param aWindowHeight The height of the window.
 */
Window::Window(int aWindowWidth, int aWindowHeight)
{
	// Initialize SDL if not already done
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		throw std::runtime_error("SDL could not initialize! SDL_Error: " + std::string(SDL_GetError()));
	}

	// Create the window
	mWindow = SDL_CreateWindow("SDL Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, aWindowWidth,
							   aWindowHeight, SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN);

	if (mWindow == nullptr)
	{
		throw std::runtime_error("Window could not be created! SDL_Error: " + std::string(SDL_GetError()));
	}
}

/**
 * @brief Destructor for the Window class. Destroys the window and quits SDL subsystems.
 */
Window::~Window()
{
	// Destroy window
	if (mWindow != nullptr)
	{
		SDL_DestroyWindow(mWindow);
	}
	// Quit SDL subsystems
	SDL_Quit();
}

/**
 * @brief Shows the window.
 */
void Window::showWindow() { SDL_ShowWindow(mWindow); }

/**
 * @brief Retrieves the size of the window.
 * @return The size of the window as a Vector2.
 */
Vector2 Window::getSize()
{
	int width, height;
	SDL_GetWindowSize(mWindow, &width, &height);
	return Vector2(width, height);
}

/**
 * @brief Sets the window to fullscreen mode.
 */
void Window::setFullScreen()
{
	if (SDL_SetWindowFullscreen(mWindow, SDL_WINDOW_FULLSCREEN) != 0)
	{
		throw std::runtime_error("Unable to set fullscreen mode! SDL_Error: " + std::string(SDL_GetError()));
	}
}

/**
 * @brief Sets the window to floating (borderless) mode.
 */
void Window::setFloating() { SDL_SetWindowBordered(mWindow, SDL_FALSE); }

/**
 * @brief Toggles the window between fullscreen and floating (borderless) mode.
 */
void Window::toggleFullFloating()
{
	Uint32 windowFlags = SDL_GetWindowFlags(mWindow);

	if (windowFlags & SDL_WINDOW_FULLSCREEN)
	{
		// If it's currently fullscreen, set it to floating (borderless)
		setFloating();
	}
	else
	{
		// Otherwise, set it to fullscreen
		setFullScreen();
	}
}

/**
 * @brief Retrieves the SDL window.
 * @return The SDL window.
 */
SDL_Window* Window::getSDLWindow() { return mWindow; }
