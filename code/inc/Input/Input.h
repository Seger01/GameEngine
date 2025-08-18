/**
 * @file Input.h
 * @brief Input class that handles all input from the user.
 * @details This class is a singleton that handles all input from the user. It is responsible for updating the current
 * state of the keyboard and mouse, as well as keeping track of the previous state of the keyboard and mouse. It also
 * provides functions to check if a key is currently being held, pressed, or released. It also provides functions to
 * check if a mouse button is currently being held, pressed, or released.
 */

#ifndef INPUT_H
#define INPUT_H

#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <SDL.h>

#include "Engine/EngineBravo.h"
#include "InputStructs.h"
#include "Rendering/Window.h"

/**
 * @brief Input class that handles all input from the user.
 * @details This class is a singleton that handles all input from the user. It is responsible for updating the current
 * state of the keyboard and mouse, as well as keeping track of the previous state of the keyboard and mouse. It also
 * provides functions to check if a key is currently being held, pressed, or released. It also provides functions to
 * check if a mouse button is currently being held, pressed, or released.
 */
class Input
{

	Input() { /* this->updateCurrentKeys();  */ }

public:
	// Get the singleton instance
	static Input& getInstance()
	{
		static Input instance;
		return instance;
	}

	// Delete copy constructor and assignment operator
	Input(const Input&) = delete;
	Input& operator=(const Input&) = delete;

	void update()
	{
		mWindow.updateEvents();

		this->updateUpKeys();
		this->updateDownKeys();
		this->updateCurrentKeys();

		this->updateMouse();
	}

	bool* getDownKeys() { return mDownKeys; }

	bool* getUpKeys() { return mUpKeys; }

	Point mousePosition() { return mCurrentMouse.position; }

	bool getKey(Key key)
	{
		if (mCurrentKeys[(int)key] == 1)
		{
			return true;
		}
		return false;
	}

	bool getKeyDown(Key key)
	{
		if (mDownKeys[(int)key] == 1)
		{
			return true;
		}
		return false;
	}

	bool getKeyUp(Key key)
	{
		if (mUpKeys[(int)key] == 1)
		{
			return true;
		}
		return false;
	}

	bool getMouseButton(MouseButton which)
	{
		switch (which)
		{
		case (MouseButton::LEFT):
			return mCurrentMouse.left;
			break;
		case (MouseButton::MIDDLE):
			return mCurrentMouse.middle;
			break;
		case (MouseButton::RIGHT):
			return mCurrentMouse.right;
			break;
		default:
			return false;
			break;
		}
		return false;
	}

	bool getMouseButtonDown(MouseButton which)
	{
		switch (which)
		{
		case (MouseButton::LEFT):
			if (mPreviousMouse.left == 0 && mCurrentMouse.left == 1)
			{
				return true;
			}
			return false;
			break;
		case (MouseButton::MIDDLE):
			if (mPreviousMouse.middle == 0 && mCurrentMouse.middle == 1)
			{
				return true;
			}
			return false;
			break;
		case (MouseButton::RIGHT):
			if (mPreviousMouse.right == 0 && mCurrentMouse.right == 1)
			{
				return true;
			}
			return false;
			break;
		default:
			return false;
			break;
		}
		return false;
	}

	/**
	 * @brief Returns true during the frame the user releases the given mouse button.
	 * @spicapi
	 */
	bool getMouseButtonUp(MouseButton which)
	{
		switch (which)
		{
		case (MouseButton::LEFT):
			if (mPreviousMouse.left == 1 && mCurrentMouse.left == 0)
			{
				return true;
			}
			return false;
			break;
		case (MouseButton::MIDDLE):
			if (mPreviousMouse.middle == 1 && mCurrentMouse.middle == 0)
			{
				return true;
			}
			return false;
			break;
		case (MouseButton::RIGHT):
			if (mPreviousMouse.right == 1 && mCurrentMouse.right == 0)
			{
				return true;
			}
			return false;
			break;
		default:
			return false;
			break;
		}
		return false;
	}

private:
	void updateCurrentKeys()
	{
		for (int i = 0; i < 512; i++)
		{
			if (mDownKeys[i] == true)
			{
				mCurrentKeys[i] = true;
			}
			else if (mUpKeys[i] == true)
			{
				mCurrentKeys[i] = false;
			}
		}
	}

	void updateDownKeys()
	{
		bool* downKeys = mWindow.getKeyPresses();

		for (int i = 0; i < 512; i++)
		{
			mDownKeys[i] = downKeys[i];
		}
	}

	void updateUpKeys()
	{
		bool* upKeys = mWindow.getKeyReleases();

		for (int i = 0; i < 512; i++)
		{
			mUpKeys[i] = upKeys[i];
		}
	}

	void updateMouse()
	{
		mPreviousMouse = mCurrentMouse;

		mCurrentMouse = Mouse();

		Vector2 mousePosition = mWindow.getMousePosition();

		mCurrentMouse.position.x = mousePosition.x;
		mCurrentMouse.position.y = mousePosition.y;

		bool* mouseButtons = mWindow.getMouseButtonsStates();

		mCurrentMouse.left = mouseButtons[0];
		mCurrentMouse.right = mouseButtons[1];
		mCurrentMouse.middle = mouseButtons[2];
	}

private:
	Mouse mCurrentMouse;
	Mouse mPreviousMouse;

	bool mCurrentKeys[512] = {0};
	bool mDownKeys[512] = {0};
	bool mUpKeys[512] = {0};

	Window& mWindow = EngineBravo::getInstance().getRenderSystem().getWindow();
};

#endif
