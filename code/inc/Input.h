#pragma once

#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <SDL.h>
#include <nlohmann/json.hpp>

#include "InputStructs.h"

class Input {
private:
    void updatePreviousKeys() {
        mPreviousKeys.resize(mNumKeys);
        for (int i = 0; i < mNumKeys; i++) {
            mPreviousKeys[i] = mCurrentKeys[i];
            // if (mCurrentKeys[i] == 1){
            //     mPreviousKeys[]
            // }
            // // mPreviousKeys = std::vector<Uint8>(mCurrentKeys, mCurrentKeys + mNumKeys);
            // }
        }
    }

    void updateCurrentKeys() { mCurrentKeys = SDL_GetKeyboardState(&mNumKeys); }

    void updateDownKeys() {
        mDownKeys.resize(0);
        // std::cout << "mPreviousKeys.size(): " << mPreviousKeys.size() << std::endl;
        for (int i = 0; i < mPreviousKeys.size(); i++) {
            if (mCurrentKeys[i] == 1 && mPreviousKeys[i] == 0) {
                mDownKeys.push_back(i);
            }
        }
    }

    void updateUpKeys() {
        mUpKeys.resize(0);

        for (int i = 0; i < mPreviousKeys.size(); i++) {
            if (mCurrentKeys[i] == 0 && mPreviousKeys[i] == 1) {
                mUpKeys.push_back(i);
            }
        }
    }

    void updateHeldKeys() {
        mHeldKeys.resize(0);

        for (int i = 0; i < mNumKeys; i++) {
            if (mCurrentKeys[i]) {
                mHeldKeys.push_back(i);
            }
        }
    }

    void updateMouse() {
        mPreviousMouse = mCurrentMouse;

        mCurrentMouse = Mouse();

        int mouseX, mouseY;
        Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);

        mCurrentMouse.position.x = mouseX;
        mCurrentMouse.position.y = mouseY;

        if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) {
            mCurrentMouse.left = true;
        }

        // Check if the right mouse button is pressed
        if (mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
            mCurrentMouse.right = true;
        }

        // Check if the middle mouse button is pressed
        if (mouseState & SDL_BUTTON(SDL_BUTTON_MIDDLE)) {
            mCurrentMouse.middle = true;
        }
    }

    Input() {
        this->updateCurrentKeys();
        this->updatePreviousKeys();
    }

public:
    // Get the singleton instance
    static Input& getInstance() {
        static Input instance;
        return instance;
    }

    // Delete copy constructor and assignment operator
    Input(const Input&) = delete;
    Input& operator=(const Input&) = delete;

    void print() {
        static long printID = 0;

        for (int i = 0; i < mNumKeys; i++) {
            if (mCurrentKeys[i] == 1) {
                std::cout << printID << ": mCurrentKeys[" << i << "] == " << keyToString((Key)i) << std::endl;
            }
        }

        for (int i = 0; i < mPreviousKeys.size(); i++) {
            if (mPreviousKeys[i] == 1) {
                std::cout << printID << ": mPrevousKeys[" << i << "] == " << keyToString((Key)i) << std::endl;
            }
        }

        printID++;
    }

    void update() {
        this->updateCurrentKeys();
        this->updateUpKeys();
        this->updateDownKeys();
        this->updateHeldKeys();

        this->updateMouse();
        this->updatePreviousKeys();
    }

    std::vector<Uint8>& getHeldKeys() { return mHeldKeys; }
    std::vector<Uint8>& getDownKeys() { return mDownKeys; }
    std::vector<Uint8>& getUpKeys() { return mUpKeys; }

    /**
     * @brief Is any key or mouse button currently held down? (Read Only)
     * @spicapi
     */
    bool AnyKey() {
        if (mHeldKeys.size() == 0)
            return false;

        return true;
    }

    /**
     * @brief Returns true the first frame the user hits any key or mouse button. (Read Only)
     * @spicapi
     */
    bool AnyKeyDown() {
        if (mDownKeys.size() == 0)
            return false;

        return true;
    }

    /**
     * @brief The current mouse position in pixel coordinates. (Read Only)
     * @spicapi
     */
    Point MousePosition() { return mCurrentMouse.position; }

    /**
     * @brief Returns the value of the virtual axis identified by axisName.
     * @spicapi
     */
    double GetAxis();

    /**
     * @brief Returns true while the user holds down the key identified by keycode.
     * @spicapi
     */
    bool GetKey(Key key) {
        for (const auto& heldKey : mHeldKeys) {
            if (heldKey == (Uint8)key) {
                return true;
            }
        }
        return false;
    }

    /**
     * @brief Returns true during the frame the user starts pressing down the key identified by keycode.
     * @spicapi
     */
    bool GetKeyDown(Key key) {
        // for (const auto& downKey : mDownKeys) {
        //     if (downKey == (Uint8)key) {
        //         return true;
        //     }
        // }
        if (mDownKeys.size() != 0)
            std::cout << "mDownKeys.size(): " << mDownKeys.size() << std::endl;

        for (int i = 0; i < mDownKeys.size(); i++) {
            if (mDownKeys[i] == (int)key) {
                return true;
            }
        }
        return false;
    }

    /**
     * @brief Returns true during the frame the user releases the key identified by keycode.
     * @spicapi
     */
    bool GetKeyUp(Key key) {
        for (const auto& upKey : mUpKeys) {
            if (upKey == (Uint8)key) {
                return true;
            }
        }
        return false;
    }

    /**
     * @brief Returns whether the given mouse button is held down.
     * @spicapi
     */
    bool GetMouseButton(MouseButton which) {
        switch (which) {
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

    /**
     * @brief Returns true during the frame the user pressed the given mouse button.
     * @spicapi
     */
    bool GetMouseButtonDown(MouseButton which) {
        switch (which) {
        case (MouseButton::LEFT):
            if (mPreviousMouse.left == 0 && mCurrentMouse.left == 1) {
                return true;
            }
            return false;
            break;
        case (MouseButton::MIDDLE):
            if (mPreviousMouse.middle == 0 && mCurrentMouse.middle == 1) {
                return true;
            }
            return false;
            break;
        case (MouseButton::RIGHT):
            if (mPreviousMouse.right == 0 && mCurrentMouse.right == 1) {
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
    bool GetMouseButtonUp(MouseButton which) {
        switch (which) {
        case (MouseButton::LEFT):
            if (mPreviousMouse.left == 1 && mCurrentMouse.left == 0) {
                return true;
            }
            return false;
            break;
        case (MouseButton::MIDDLE):
            if (mPreviousMouse.middle == 1 && mCurrentMouse.middle == 0) {
                return true;
            }
            return false;
            break;
        case (MouseButton::RIGHT):
            if (mPreviousMouse.right == 1 && mCurrentMouse.right == 0) {
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
    Mouse mCurrentMouse;
    Mouse mPreviousMouse;

    const Uint8* mCurrentKeys = nullptr;
    int mNumKeys = 0;

    std::vector<Uint8> mPreviousKeys;
    std::vector<Uint8> mDownKeys;
    std::vector<Uint8> mUpKeys;
    std::vector<Uint8> mHeldKeys;
};
