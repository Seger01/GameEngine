#include "Rendering/Window.h"
#include <stdexcept>

#include <iostream>

/**
 * @brief Constructor for the Window class. Initializes the window with a specified width and height.
 * @param aWindowWidth The width of the window.
 * @param aWindowHeight The height of the window.
 */
Window::Window(int aWindowWidth, int aWindowHeight) : mWindow(aWindowWidth, aWindowHeight)
{
	std::cout << "Window constructor engine called" << std::endl;
}

/**
 * @brief Destructor for the Window class. Destroys the window and quits SDL subsystems.
 */
Window::~Window() {}

/**
 * @brief Shows the window.
 */
void Window::showWindow() { /*  SDL_ShowWindow(mWindow);  */ }

/**
 * @brief Retrieves the size of the window.
 * @return The size of the window as a Vector2.
 */
Vector2 Window::getSize()
{
	SML::SML_Point size = mWindow.getWindowSize();

	Vector2 sizeVector(size.x, size.y);

	return sizeVector;
}

/**
 * @brief Sets the size of the window.
 * @param aSize The new size of the window.
 */
void Window::setSize(Vector2 aSize) { /*  SDL_SetWindowSize(mWindow, aSize.x, aSize.y);  */ }

/**
 * @brief Sets the window to fullscreen mode.
 */
void Window::setFullScreen()
{
	// if (SDL_SetWindowFullscreen(mWindow, SDL_WINDOW_FULLSCREEN) != 0)
	// {
	// 	throw std::runtime_error("Unable to set fullscreen mode! SDL_Error: " + std::string(SDL_GetError()));
	// }
}

/**
 * @brief Sets the window to floating (borderless) mode.
 */
void Window::setFloating() { /*  SDL_SetWindowBordered(mWindow, SDL_FALSE);  */ }

/**
 * @brief Toggles the window between fullscreen and floating (borderless) mode.
 */
void Window::toggleFullFloating()
{
	// Uint32 windowFlags = SDL_GetWindowFlags(mWindow);
	//
	// if (windowFlags & SDL_WINDOW_FULLSCREEN)
	// {
	// 	// If it's currently fullscreen, set it to floating (borderless)
	// 	setFloating();
	// }
	// else
	// {
	// 	// Otherwise, set it to fullscreen
	// 	setFullScreen();
	// }
}

void Window::updateEvents() { mWindow.updateEvents(); }

bool Window::shouldClose()
{
	// Check if the window should close
	return mWindow.shouldClose();
}

bool* Window::getKeyPresses()
{
	// Get the current key states
	return mWindow.getKeyPresses();
}

bool* Window::getKeyReleases()
{
	// Get the current key release states
	return mWindow.getKeyReleases();
}

bool* Window::getMouseButtonsStates()
{
	// Get the current mouse button states
	return mWindow.getMouseButtonsStates();
}

Vector2 Window::getMousePosition()
{
	// Get the current mouse position
	SML::Vector2 mousePos = mWindow.getMousePosition();
	return Vector2(mousePos.x, mousePos.y);
}
