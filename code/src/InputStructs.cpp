#include "InputStructs.h"

// Function to convert enum class values to their corresponding string representation
std::string keyToString(Key key) {
    switch (key) {
    // Alphabet keys
    case Key::Key_A:
        return "Key_A";
    case Key::Key_B:
        return "Key_B";
    case Key::Key_C:
        return "Key_C";
    case Key::Key_D:
        return "Key_D";
    case Key::Key_E:
        return "Key_E";
    case Key::Key_F:
        return "Key_F";
    case Key::Key_G:
        return "Key_G";
    case Key::Key_H:
        return "Key_H";
    case Key::Key_I:
        return "Key_I";
    case Key::Key_J:
        return "Key_J";
    case Key::Key_K:
        return "Key_K";
    case Key::Key_L:
        return "Key_L";
    case Key::Key_M:
        return "Key_M";
    case Key::Key_N:
        return "Key_N";
    case Key::Key_O:
        return "Key_O";
    case Key::Key_P:
        return "Key_P";
    case Key::Key_Q:
        return "Key_Q";
    case Key::Key_R:
        return "Key_R";
    case Key::Key_S:
        return "Key_S";
    case Key::Key_T:
        return "Key_T";
    case Key::Key_U:
        return "Key_U";
    case Key::Key_V:
        return "Key_V";
    case Key::Key_W:
        return "Key_W";
    case Key::Key_X:
        return "Key_X";
    case Key::Key_Y:
        return "Key_Y";
    case Key::Key_Z:
        return "Key_Z";

    // Number keys (top row)
    case Key::Key_0:
        return "Key_0";
    case Key::Key_1:
        return "Key_1";
    case Key::Key_2:
        return "Key_2";
    case Key::Key_3:
        return "Key_3";
    case Key::Key_4:
        return "Key_4";
    case Key::Key_5:
        return "Key_5";
    case Key::Key_6:
        return "Key_6";
    case Key::Key_7:
        return "Key_7";
    case Key::Key_8:
        return "Key_8";
    case Key::Key_9:
        return "Key_9";

    // Function keys
    case Key::Key_F1:
        return "Key_F1";
    case Key::Key_F2:
        return "Key_F2";
    case Key::Key_F3:
        return "Key_F3";
    case Key::Key_F4:
        return "Key_F4";
    case Key::Key_F5:
        return "Key_F5";
    case Key::Key_F6:
        return "Key_F6";
    case Key::Key_F7:
        return "Key_F7";
    case Key::Key_F8:
        return "Key_F8";
    case Key::Key_F9:
        return "Key_F9";
    case Key::Key_F10:
        return "Key_F10";
    case Key::Key_F11:
        return "Key_F11";
    case Key::Key_F12:
        return "Key_F12";

    // Numpad keys
    case Key::Key_Num0:
        return "Key_Num0";
    case Key::Key_Num1:
        return "Key_Num1";
    case Key::Key_Num2:
        return "Key_Num2";
    case Key::Key_Num3:
        return "Key_Num3";
    case Key::Key_Num4:
        return "Key_Num4";
    case Key::Key_Num5:
        return "Key_Num5";
    case Key::Key_Num6:
        return "Key_Num6";
    case Key::Key_Num7:
        return "Key_Num7";
    case Key::Key_Num8:
        return "Key_Num8";
    case Key::Key_Num9:
        return "Key_Num9";
    // case Key::Key_NumLock:
    //     return "Key_NumLock";
    case Key::Key_NumDivide:
        return "Key_NumDivide";
    case Key::Key_NumMultiply:
        return "Key_NumMultiply";
    case Key::Key_NumMinus:
        return "Key_NumMinus";
    case Key::Key_NumPlus:
        return "Key_NumPlus";
    case Key::Key_NumEnter:
        return "Key_NumEnter";
    // case Key::Key_NumDecimal:
    //     return "Key_NumDecimal";

    // Arrow keys
    case Key::Key_Up:
        return "Key_Up";
    case Key::Key_Down:
        return "Key_Down";
    case Key::Key_Left:
        return "Key_Left";
    case Key::Key_Right:
        return "Key_Right";

    // Modifier keys
    case Key::Key_LShift:
        return "Key_LShift";
    case Key::Key_LControl:
        return "Key_LControl";
    case Key::Key_LAlt:
        return "Key_LAlt";
    case Key::Key_CapsLock:
        return "Key_CapsLock";

    // Special keys
    case Key::Key_Space:
        return "Key_Space";
    case Key::Key_Enter:
        return "Key_Enter";
    case Key::Key_Backspace:
        return "Key_Backspace";
    case Key::Key_Tab:
        return "Key_Tab";
    case Key::Key_Escape:
        return "Key_Escape";
    case Key::Key_Insert:
        return "Key_Insert";
    case Key::Key_Delete:
        return "Key_Delete";
    case Key::Key_Home:
        return "Key_Home";
    case Key::Key_End:
        return "Key_End";
    case Key::Key_PageUp:
        return "Key_PageUp";
    case Key::Key_PageDown:
        return "Key_PageDown";
    case Key::Key_PrintScreen:
        return "Key_PrintScreen";
    case Key::Key_ScrollLock:
        return "Key_ScrollLock";
    case Key::Key_Pause:
        return "Key_Pause";

    // Punctuation keys
    // case Key::Key_Tilde:
    //     return "Key_Tilde";
    case Key::Key_Minus:
        return "Key_Minus";
    case Key::Key_Equals:
        return "Key_Equals";
    case Key::Key_LeftBracket:
        return "Key_LeftBracket";
    case Key::Key_RightBracket:
        return "Key_RightBracket";
    case Key::Key_Backslash:
        return "Key_Backslash";
    case Key::Key_Semicolon:
        return "Key_Semicolon";
    // case Key::Key_Quote:
    //     return "Key_Quote";
    case Key::Key_Comma:
        return "Key_Comma";
    case Key::Key_Period:
        return "Key_Period";
    case Key::Key_Slash:
        return "Key_Slash";

    // Mouse buttons
    case Key::Mouse_LeftButton:
        return "Mouse_LeftButton";
    case Key::Mouse_RightButton:
        return "Mouse_RightButton";
    case Key::Mouse_MiddleButton:
        return "Mouse_MiddleButton";

    // Controller buttons
    case Key::Controller_A:
        return "Controller_A";
    case Key::Controller_B:
        return "Controller_B";
    case Key::Controller_X:
        return "Controller_X";
    case Key::Controller_Y:
        return "Controller_Y";
    case Key::Controller_LeftBumper:
        return "Controller_LeftBumper";
    case Key::Controller_RightBumper:
        return "Controller_RightBumper";
    case Key::Controller_Back:
        return "Controller_Back";
    case Key::Controller_Start:
        return "Controller_Start";
    case Key::Controller_LeftStick_Down:
        return "Controller_LeftStick_Down";
    case Key::Controller_RightStick_Down:
        return "Controller_RightStick_Down";
    case Key::Controller_DPad_Up:
        return "Controller_DPad_Up";
    case Key::Controller_DPad_Down:
        return "Controller_DPad_Down";
    case Key::Controller_DPad_Left:
        return "Controller_DPad_Left";
    case Key::Controller_DPad_Right:
        return "Controller_DPad_Right";

        // Unknown key (default case)
    default:
        return "Unknown Key";
    }
}

std::string actionToString(DefAction action) {
    switch (action) {
    case DefAction::Move_Up:
        return "Move_Up";
    case DefAction::Move_Left:
        return "Move_Left";
    case DefAction::Move_Down:
        return "Move_Down";
    case DefAction::Move_Right:
        return "Move_Right";

    default:
        return "Unknown Action";
    }
}

int stringToActionID(std::string aKeyString) {
    std::string returnedString = "";
    int currentKeyAttempt = 0;

    do {
        returnedString = actionToString(static_cast<DefAction>(currentKeyAttempt));

        if (returnedString == aKeyString) {
            return currentKeyAttempt;
        }

        currentKeyAttempt++;

    } while (returnedString != "Unknown Action");

    return currentKeyAttempt;
}

int stringToKeyID(std::string aKeyString) {
    std::string returnedString = "";

    for (int i = 0; i < (int)Key::Key_NumberOfKeys; i++) {
        returnedString = keyToString(static_cast<Key>(i));

        if (returnedString == aKeyString) {
            return i;
        }
    }

    return 0;
}
