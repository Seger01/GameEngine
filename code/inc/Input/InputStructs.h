/**
 * @file InputStructs.h
 * @brief Contains all the structs used for input handling.
 * @details This file contains all the structs used for input handling. This includes the Mouse struct, which contains
 * the current position of the mouse, as well as the state of the left, middle, and right mouse buttons. It also
 * contains the MouseButton enum class, which contains the possible mouse buttons. It also contains the Key enum class,
 * which contains all the possible keys that can be pressed on a keyboard.
 */

#ifndef INPUTSTRUCTS_H
#define INPUTSTRUCTS_H

#include <string>

#include "Point.h"

/**
 * @brief Enum class for all mouse buttons.
 * @details This enum class contains all the possible mouse buttons that can be pressed. This includes the left, middle,
 * and right mouse buttons.
 */
enum class MouseButton
{
	LEFT = 1,
	MIDDLE = 2,
	RIGHT = 3
};

/**
 * @brief Struct that contains the current state of the mouse.
 * @details This struct contains the current position of the mouse, as well as the state of the left, middle, and right
 * mouse buttons.
 */
struct Mouse
{
	Point position;

	bool left = false;
	bool middle = false;
	bool right = false;
};

/**
 * @brief Enum class for all possible keys.
 * @details This enum class contains all the possible keys that can be pressed on a keyboard. This includes all the
 * letters, numbers, and special keys on a keyboard.
 */
enum class Key
{
	Key_UNKNOWN = -1,

	// Letters
	Key_A = 65,
	Key_B = 66,
	Key_C = 67,
	Key_D = 68,
	Key_E = 69,
	Key_F = 70,
	Key_G = 71,
	Key_H = 72,
	Key_I = 73,
	Key_J = 74,
	Key_K = 75,
	Key_L = 76,
	Key_M = 77,
	Key_N = 78,
	Key_O = 79,
	Key_P = 80,
	Key_Q = 81,
	Key_R = 82,
	Key_S = 83,
	Key_T = 84,
	Key_U = 85,
	Key_V = 86,
	Key_W = 87,
	Key_X = 88,
	Key_Y = 89,
	Key_Z = 90,

	// Numbers
	Key_0 = 48,
	Key_1 = 49,
	Key_2 = 50,
	Key_3 = 51,
	Key_4 = 52,
	Key_5 = 53,
	Key_6 = 54,
	Key_7 = 55,
	Key_8 = 56,
	Key_9 = 57,

	// Printable keys
	Key_Space = 32,
	Key_Apostrophe = 39,
	Key_Comma = 44,
	Key_Minus = 45,
	Key_Period = 46,
	Key_Slash = 47,
	Key_Semicolon = 59,
	Key_Equals = 61,
	Key_LeftBracket = 91,
	Key_Backslash = 92,
	Key_RightBracket = 93,
	Key_Grave = 96,

	// Function keys
	Key_Escape = 256,
	Key_Enter = 257,
	Key_Tab = 258,
	Key_Backspace = 259,
	Key_Insert = 260,
	Key_Delete = 261,
	Key_Right = 262,
	Key_Left = 263,
	Key_Down = 264,
	Key_Up = 265,
	Key_PageUp = 266,
	Key_PageDown = 267,
	Key_Home = 268,
	Key_End = 269,

	// Locks & Print
	Key_CapsLock = 280,
	Key_ScrollLock = 281,
	Key_NumLock = 282,
	Key_PrintScreen = 283,
	Key_Pause = 284,

	// F-keys
	Key_F1 = 290,
	Key_F2 = 291,
	Key_F3 = 292,
	Key_F4 = 293,
	Key_F5 = 294,
	Key_F6 = 295,
	Key_F7 = 296,
	Key_F8 = 297,
	Key_F9 = 298,
	Key_F10 = 299,
	Key_F11 = 300,
	Key_F12 = 301,

	// Keypad
	Key_KP_0 = 320,
	Key_KP_1 = 321,
	Key_KP_2 = 322,
	Key_KP_3 = 323,
	Key_KP_4 = 324,
	Key_KP_5 = 325,
	Key_KP_6 = 326,
	Key_KP_7 = 327,
	Key_KP_8 = 328,
	Key_KP_9 = 329,
	Key_KP_Decimal = 330,
	Key_KP_Divide = 331,
	Key_KP_Multiply = 332,
	Key_KP_Subtract = 333,
	Key_KP_Add = 334,
	Key_KP_Enter = 335,
	Key_KP_Equal = 336,

	// Modifiers
	Key_LShift = 340,
	Key_LControl = 341,
	Key_LAlt = 342,
	Key_LSuper = 343,
	Key_RShift = 344,
	Key_RControl = 345,
	Key_RAlt = 346,
	Key_RSuper = 347,
	Key_Menu = 348,

	Key_NumberOfKeys = 512
};

#endif
