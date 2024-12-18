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

#include "InputStructs.h"

/**
 * @brief Input class that handles all input from the user.
 * @details This class is a singleton that handles all input from the user. It is responsible for updating the current
 * state of the keyboard and mouse, as well as keeping track of the previous state of the keyboard and mouse. It also
 * provides functions to check if a key is currently being held, pressed, or released. It also provides functions to
 * check if a mouse button is currently being held, pressed, or released.
 */
class Input
{

	Input()
	{
		this->updateCurrentKeys();
		this->updatePreviousKeys();
	}

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
		this->updateCurrentKeys();
		this->updateUpKeys();
		this->updateDownKeys();
		this->updateHeldKeys();

		this->updateMouse();
		this->updatePreviousKeys();
	}

	std::vector<Key>& getHeldKeys() { return mHeldKeys; }

	std::vector<Key>& getDownKeys() { return mDownKeys; }

	std::vector<Key>& getUpKeys() { return mUpKeys; }

	bool AnyKey()
	{
		if (mHeldKeys.size() == 0)
			return false;

		return true;
	}

	bool AnyKeyDown()
	{
		if (mDownKeys.size() == 0)
			return false;

		return true;
	}

	Point MousePosition() { return mCurrentMouse.position; }

	bool GetKey(Key key)
	{
		for (const auto& heldKey : mHeldKeys)
		{
			if (heldKey == key)
			{
				return true;
			}
		}
		return false;
	}

	bool GetKeyDown(Key key)
	{
		if (mDownKeys.size() != 0)

			for (int i = 0; i < mDownKeys.size(); i++)
			{
				if (mDownKeys[i] == key)
				{
					return true;
				}
			}
		return false;
	}

	bool GetKeyUp(Key key)
	{
		for (const auto& upKey : mUpKeys)
		{
			if (upKey == key)
			{
				return true;
			}
		}
		return false;
	}

	bool GetMouseButton(MouseButton which)
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

	bool GetMouseButtonDown(MouseButton which)
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
	bool GetMouseButtonUp(MouseButton which)
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
	void updatePreviousKeys()
	{
		mPreviousKeys.resize(mCurrentKeys.size());
		for (int i = 0; i < mCurrentKeys.size(); i++)
		{
			mPreviousKeys[i] = (Key)mCurrentKeys[i];
		}
	}

	void updateCurrentKeys()
	{
		int numKeys = 0;
		const Uint8* currentKeys = SDL_GetKeyboardState(&numKeys);

		mCurrentKeys.resize(numKeys);

		for (int i = 0; i < numKeys; i++)
		{
			mCurrentKeys[i] = currentKeys[i];
		}
	}

	void updateDownKeys()
	{
		mDownKeys.resize(0);
		// std::cout << "mPreviousKeys.size(): " << mPreviousKeys.size() << std::endl;
		for (int i = 0; i < mPreviousKeys.size(); i++)
		{
			if (mCurrentKeys[i] == 1 && (int)mPreviousKeys[i] == 0)
			{
				mDownKeys.push_back((Key)i);
			}
		}
	}

	void updateUpKeys()
	{
		mUpKeys.resize(0);

		for (int i = 0; i < mPreviousKeys.size(); i++)
		{
			if (mCurrentKeys[i] == 0 && (int)mPreviousKeys[i] == 1)
			{
				mUpKeys.push_back((Key)i);
			}
		}
	}

	void updateHeldKeys()
	{
		mHeldKeys.resize(0);

		for (int i = 0; i < mCurrentKeys.size(); i++)
		{
			if (mCurrentKeys[i])
			{

				mHeldKeys.push_back((Key)i);
			}
		}
	}

	void updateMouse()
	{
		mPreviousMouse = mCurrentMouse;

		mCurrentMouse = Mouse();

		int mouseX, mouseY;
		Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);

		mCurrentMouse.position.x = mouseX;
		mCurrentMouse.position.y = mouseY;

		if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT))
		{
			mCurrentMouse.left = true;
		}

		// Check if the right mouse button is pressed
		if (mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT))
		{
			mCurrentMouse.right = true;
		}

		// Check if the middle mouse button is pressed
		if (mouseState & SDL_BUTTON(SDL_BUTTON_MIDDLE))
		{
			mCurrentMouse.middle = true;
		}
	}

private:
	Mouse mCurrentMouse;
	Mouse mPreviousMouse;

	std::vector<int> mCurrentKeys;

	std::vector<Key> mPreviousKeys;
	std::vector<Key> mDownKeys;
	std::vector<Key> mUpKeys;
	std::vector<Key> mHeldKeys;

	std::vector<DefAction> mHeldActions;
};

#endif
