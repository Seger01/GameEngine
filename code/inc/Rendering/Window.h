/**
 * @file Window.h
 * @brief Header file for the Window class, responsible for creating and managing the game window.
 */

#ifndef WINDOW_H
#define WINDOW_H

#include "SML/Window.h"

#include "Structs/Vector2.h"

/**
 * @class Window
 * @brief Handles the creation and management of the game window.
 *
 * This class is responsible for creating and managing the game window, including setting the window size,
 * toggling fullscreen mode, and setting the window to floating mode.
 */
class Window
{
public:
	Window(int aWindowWidth, int aWindowHeight);
	~Window();

	void showWindow();

	Vector2 getSize();
	void setSize(Vector2 aSize);

	void setFullScreen();
	void setFloating();
	void toggleFullFloating();

	SML::Window& getWindow() { return mWindow; }

	void updateEvents();
	bool shouldClose();

	bool* getKeyPresses();
	bool* getKeyReleases();

	bool* getMouseButtonsStates();

	Vector2 getMousePosition();

private:
	SML::Window mWindow;
};

#endif
