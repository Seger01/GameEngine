#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <SDL.h>

#include "Point.h"

enum class Key;
enum class DefAction;

// Function to convert enum class values to their corresponding string representation
std::string keyToString(Key key);

int stringToKeyID(std::string aKeyString);

std::string actionToString(DefAction action);

int stringToActionID(std::string aKeyString);

enum class MouseButton { LEFT = 1, MIDDLE = 2, RIGHT = 3 };

struct Mouse {
    Point position;

    bool left = false;
    bool middle = false;
    bool right = false;
};

// Define an enum class for all keyboard, mouse, and controller keys
enum class Key {
    Key_UNKNOWN = 0,

    /**
     *  \name Usage page 0x07
     *
     *  These values are from usage page 0x07 (USB keyboard page).
     */
    /*  */

    Key_A = 4,
    Key_B = 5,
    Key_C = 6,
    Key_D = 7,
    Key_E = 8,
    Key_F = 9,
    Key_G = 10,
    Key_H = 11,
    Key_I = 12,
    Key_J = 13,
    Key_K = 14,
    Key_L = 15,
    Key_M = 16,
    Key_N = 17,
    Key_O = 18,
    Key_P = 19,
    Key_Q = 20,
    Key_R = 21,
    Key_S = 22,
    Key_T = 23,
    Key_U = 24,
    Key_V = 25,
    Key_W = 26,
    Key_X = 27,
    Key_Y = 28,
    Key_Z = 29,

    Key_1 = 30,
    Key_2 = 31,
    Key_3 = 32,
    Key_4 = 33,
    Key_5 = 34,
    Key_6 = 35,
    Key_7 = 36,
    Key_8 = 37,
    Key_9 = 38,
    Key_0 = 39,

    Key_Enter = 40,
    Key_Escape = 41,
    Key_Backspace = 42,
    Key_Tab = 43,
    Key_Space = 44,

    Key_Minus = 45,
    Key_Equals = 46,
    Key_LeftBracket = 47,
    Key_RightBracket = 48,
    Key_Backslash = 49, /**< Located at the lower left of the return
                         *   key on ISO keyboards and at the right end
                         *   of the QWERTY row on ANSI keyboards.
                         *   Produces REVERSE SOLIDUS (backslash) and
                         *   VERTICAL LINE in a US layout, REVERSE
                         *   SOLIDUS and VERTICAL LINE in a UK Mac
                         *   layout, NUMBER SIGN and TILDE in a UK
                         *   Windows layout, DOLLAR SIGN and POUND SIGN
                         *   in a Swiss German layout, NUMBER SIGN and
                         *   APOSTROPHE in a German layout, GRAVE
                         *   ACCENT and POUND SIGN in a French Mac
                         *   layout, and ASTERISK and MICRO SIGN in a
                         *   French Windows layout.
                         */
    Key_NONUSHASH = 50, /**< ISO USB keyboards actually use this code
                         *   instead of 49 for the same key, but all
                         *   OSes I've seen treat the two codes
                         *   identically. So, as an implementor, unless
                         *   your keyboard generates both of those
                         *   codes and your OS treats them differently,
                         *   you should generate Key_BACKSLASH
                         *   instead of this code. As a user, you
                         *   should not rely on this code because SDL
                         *   will never generate it with most (all?)
                         *   keyboards.
                         */
    Key_Semicolon = 51,
    Key_Apostrophe = 52,
    Key_Grave = 53, /**< Located in the top left corner (on both ANSI
                     *   and ISO keyboards). Produces GRAVE ACCENT and
                     *   TILDE in a US Windows layout and in US and UK
                     *   Mac layouts on ANSI keyboards, GRAVE ACCENT
                     *   and NOT SIGN in a UK Windows layout, SECTION
                     *   SIGN and PLUS-MINUS SIGN in US and UK Mac
                     *   layouts on ISO keyboards, SECTION SIGN and
                     *   DEGREE SIGN in a Swiss German layout (Mac:
                     *   only on ISO keyboards), CIRCUMFLEX ACCENT and
                     *   DEGREE SIGN in a German layout (Mac: only on
                     *   ISO keyboards), SUPERSCRIPT TWO and TILDE in a
                     *   French Windows layout, COMMERCIAL AT and
                     *   NUMBER SIGN in a French Mac layout on ISO
                     *   keyboards, and LESS-THAN SIGN and GREATER-THAN
                     *   SIGN in a Swiss German, German, or French Mac
                     *   layout on ANSI keyboards.
                     */
    Key_Comma = 54,
    Key_Period = 55,
    Key_Slash = 56,

    Key_CapsLock = 57,

    Key_F1 = 58,
    Key_F2 = 59,
    Key_F3 = 60,
    Key_F4 = 61,
    Key_F5 = 62,
    Key_F6 = 63,
    Key_F7 = 64,
    Key_F8 = 65,
    Key_F9 = 66,
    Key_F10 = 67,
    Key_F11 = 68,
    Key_F12 = 69,

    Key_PrintScreen = 70,
    Key_ScrollLock = 71,
    Key_Pause = 72,
    Key_Insert = 73, /**< insert on PC, help on some Mac keyboards (but
                                   does send code 73, not 117) */
    Key_Home = 74,
    Key_PageUp = 75,
    Key_Delete = 76,
    Key_End = 77,
    Key_PageDown = 78,
    Key_Right = 79,
    Key_Left = 80,
    Key_Down = 81,
    Key_Up = 82,

    Key_NUMLOCKCLEAR = 83, /**< num lock on PC, clear on Mac keyboards
                            */
    Key_NumDivide = 84,
    Key_NumMultiply = 85,
    Key_NumMinus = 86,
    Key_NumPlus = 87,
    Key_NumEnter = 88,
    Key_Num1 = 89,
    Key_Num2 = 90,
    Key_Num3 = 91,
    Key_Num4 = 92,
    Key_Num5 = 93,
    Key_Num6 = 94,
    Key_Num7 = 95,
    Key_Num8 = 96,
    Key_Num9 = 97,
    Key_Num0 = 98,
    Key_KP_PERIOD = 99,

    Key_LControl = 224,
    Key_LShift = 225,
    Key_LAlt = 226, /**< alt, option */
    Key_LGui = 227, /**< windows, command (apple), meta */
    Key_RControl = 228,
    Key_RShift = 229,
    Key_RAlt = 230, /**< alt gr, option */
    Key_RGui = 231, /**< windows, command (apple), meta */

    // Mouse buttons
    Mouse_LeftButton,
    Mouse_RightButton,
    Mouse_MiddleButton,

    // Controller buttons
    Controller_A,
    Controller_B,
    Controller_X,
    Controller_Y,
    Controller_LeftBumper,
    Controller_RightBumper,
    Controller_Back,
    Controller_Start,
    Controller_LeftStick,
    Controller_RightStick,
    Controller_DPad_Up,
    Controller_DPad_Down,
    Controller_DPad_Left,
    Controller_DPad_Right,

    Key_NumberOfKeys = 512 /**< not a key, just marks the number of scancodes
                                 for array bounds */

};

enum class DefAction {
    Move_Up,
    Move_Left,
    Move_Down,
    Move_Right,

    Undefined
};

class Input {
private:
    void updatePreviousKeys() {
        mPreviousKeys.resize(mNumKeys);
        for (int i = 0; i < mNumKeys; i++) {
            mPreviousKeys[i] = mCurrentKeys[i];
            // if (mCurrentKeys[i] == 1){
            //     mPreviousKeys[]
            // }
            // // mPreviousKeys = std::vector<Uint8>(mCurrentKeys, mCurrentKeys + mNumKeys); }
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

    bool getAction(DefAction aAction) { return false; }

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
