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
#include <nlohmann/json.hpp>

#include "InputStructs.h"

#include "ContextManager.h"

// class ContextManager;

class Input {
private:
    void updatePreviousKeys() {
        mPreviousKeys.resize(mCurrentKeys.size());
        for (int i = 0; i < mCurrentKeys.size(); i++) {
            mPreviousKeys[i] = (Key)mCurrentKeys[i];
            // if (mCurrentKeys[i] == 1){
            //     mPreviousKeys[]
            // }
            // // mPreviousKeys = std::vector<Uint8>(mCurrentKeys, mCurrentKeys + mNumKeys);
            // }
        }
    }

    void updateCurrentKeys() {
        int numKeys = 0;
        const Uint8* currentKeys = SDL_GetKeyboardState(&numKeys);

        mCurrentKeys.resize(numKeys);

        for (int i = 0; i < numKeys; i++) {
            mCurrentKeys[i] = currentKeys[i];
        }
    }

    void updateDownKeys() {
        mDownKeys.resize(0);
        // std::cout << "mPreviousKeys.size(): " << mPreviousKeys.size() << std::endl;
        for (int i = 0; i < mPreviousKeys.size(); i++) {
            if (mCurrentKeys[i] == 1 && (int)mPreviousKeys[i] == 0) {
                mDownKeys.push_back((Key)i);
            }
        }
    }

    void updateUpKeys() {
        mUpKeys.resize(0);

        for (int i = 0; i < mPreviousKeys.size(); i++) {
            if (mCurrentKeys[i] == 0 && (int)mPreviousKeys[i] == 1) {
                mUpKeys.push_back((Key)i);
            }
        }
    }

    void updateHeldKeys() {
        mHeldKeys.resize(0);

        for (int i = 0; i < mCurrentKeys.size(); i++) {
            if (mCurrentKeys[i]) {

                mHeldKeys.push_back((Key)i);
            }
        }
        std::cout << "mHeldKeys.size(): " << mHeldKeys.size() << std::endl;
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

    void updateHeldActions() {

        std::cout << "mHeldKeysSize from fucking input class: " << mHeldKeys.size() << std::endl;
        this->mHeldActions = mContextManager.getCurrentActions(mHeldKeys);
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

        for (int i = 0; i < mCurrentKeys.size(); i++) {
            if (mCurrentKeys[i] == 1) {
                std::cout << printID << ": mCurrentKeys[" << i << "] == " << keyToString((Key)i) << std::endl;
            }
        }

        for (int i = 0; i < mPreviousKeys.size(); i++) {
            if ((int)mPreviousKeys[i] == 1) {
                std::cout << printID << ": mPrevousKeys[" << i << "] == " << keyToString((Key)i) << std::endl;
            }
        }

        printID++;
    }

    void update() {
        this->updateCurrentKeys();
        // this->updateUpKeys();
        // this->updateDownKeys();
        this->updateHeldKeys();

        // this->updateHeldActions();

        // this->updateMouse();
        this->updatePreviousKeys();
    }

    std::vector<Key>& getHeldKeys() { return mHeldKeys; }
    std::vector<Key>& getDownKeys() { return mDownKeys; }
    std::vector<Key>& getUpKeys() { return mUpKeys; }

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

    bool GetAction(DefAction aDefAction) {
        // std::cout << "mHeldAction.size(): " << mHeldActions.size() << std::endl;
        for (auto& action : mHeldActions) {
            if (aDefAction == action) {
                return true;
            }
        }

        return false;
    }

    /**
     * @brief Returns true while the user holds down the key identified by keycode.
     * @spicapi
     */
    bool GetKey(Key key) {
        for (const auto& heldKey : mHeldKeys) {
            if (heldKey == key) {
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
            if (mDownKeys[i] == key) {
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
            if (upKey == key) {
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

    void setActiveContext(std::string aContextName) { mContextManager.setActiveContext(aContextName); }

private:
    ContextManager mContextManager;

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
