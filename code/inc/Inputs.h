#pragma once

#include <string>

enum class Key;
enum class DefAction;

// Function to convert enum class values to their corresponding string representation
std::string keyToString(Key key);

int stringToKeyID(std::string aKeyString);

std::string actionToString(DefAction action);

int stringToActionID(std::string aKeyString);

// Define an enum class for all keyboard, mouse, and controller keys
enum class Key {
    // Alphabet keys
    Key_A,
    Key_B,
    Key_C,
    Key_D,
    Key_E,
    Key_F,
    Key_G,
    Key_H,
    Key_I,
    Key_J,
    Key_K,
    Key_L,
    Key_M,
    Key_N,
    Key_O,
    Key_P,
    Key_Q,
    Key_R,
    Key_S,
    Key_T,
    Key_U,
    Key_V,
    Key_W,
    Key_X,
    Key_Y,
    Key_Z,

    // Number keys (top row)
    Key_0,
    Key_1,
    Key_2,
    Key_3,
    Key_4,
    Key_5,
    Key_6,
    Key_7,
    Key_8,
    Key_9,

    // Function keys
    Key_F1,
    Key_F2,
    Key_F3,
    Key_F4,
    Key_F5,
    Key_F6,
    Key_F7,
    Key_F8,
    Key_F9,
    Key_F10,
    Key_F11,
    Key_F12,

    // Numpad keys
    Key_Num0,
    Key_Num1,
    Key_Num2,
    Key_Num3,
    Key_Num4,
    Key_Num5,
    Key_Num6,
    Key_Num7,
    Key_Num8,
    Key_Num9,
    Key_NumLock,
    Key_NumDivide,
    Key_NumMultiply,
    Key_NumMinus,
    Key_NumPlus,
    Key_NumEnter,
    Key_NumDecimal,

    // Arrow keys
    Key_Up,
    Key_Down,
    Key_Left,
    Key_Right,

    // Modifier keys
    Key_Shift,
    Key_Control,
    Key_Alt,
    Key_CapsLock,

    // Special keys
    Key_Space,
    Key_Enter,
    Key_Backspace,
    Key_Tab,
    Key_Escape,
    Key_Insert,
    Key_Delete,
    Key_Home,
    Key_End,
    Key_PageUp,
    Key_PageDown,
    Key_PrintScreen,
    Key_ScrollLock,
    Key_Pause,

    // Punctuation keys
    Key_Tilde,
    Key_Minus,
    Key_Equals,
    Key_LeftBracket,
    Key_RightBracket,
    Key_Backslash,
    Key_Semicolon,
    Key_Quote,
    Key_Comma,
    Key_Period,
    Key_Slash,

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

    // Undefined key (fallback)
    Unknown
};

enum class DefAction {
    Move_Up,
    Move_Left,
    Move_Down,
    Move_Right,

    Undefined
};
