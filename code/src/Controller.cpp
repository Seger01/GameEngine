#include <cmath>
#include <iostream>
#include <map>

#include "Controller.h"

const int LEFT_STICK_UP_AXIS = 1;
const int LEFT_STICK_LEFT_AXIS = 0;
const int LEFT_STICK_DOWN_AXIS = 1;
const int LEFT_STICK_RIGHT_AXIS = 0;

const int RIGHT_STICK_UP_AXIS = 3;
const int RIGHT_STICK_LEFT_AXIS = 2;
const int RIGHT_STICK_DOWN_AXIS = 3;
const int RIGHT_STICK_RIGHT_AXIS = 2;

const int DPadNothing = -1;
const int DPadUp = 0;
const int DPadLeft = 1;
const int DPadDown = 2;
const int DPadRight = 3;

Controller::Controller(int index) : joystickIndex(index), joystick(nullptr), connected(false)
{
	// if (SDL_Init(SDL_INIT_GAMECONTROLLER) < 0)
	// {
	// 	std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
	// 	return;
	// }
	//
	// if (SDL_NumJoysticks() > joystickIndex)
	// {
	// 	joystick = SDL_JoystickOpen(joystickIndex);
	// 	if (joystick)
	// 	{
	// 		connected = true;
	// 		axisValues.resize(SDL_JoystickNumAxes(joystick), 0);
	// 		buttonStates.resize(SDL_JoystickNumButtons(joystick), false);
	// 	}
	// 	else
	// 	{
	// 		std::cerr << "Failed to open joystick: " << SDL_GetError() << std::endl;
	// 	}
	// }
	// else
	// {
	// 	std::cerr << "No joystick connected at index " << joystickIndex << std::endl;
	// }
}

Controller::~Controller()
{
	if (joystick)
	{
		SDL_JoystickClose(joystick);
	}
}

bool Controller::isConnected() const { return connected; }

void Controller::update()
{
	if (!connected)
		return;

	previousButtonStates = buttonStates;

	SDL_JoystickUpdate();
	pollControllerState();
}

void Controller::pollDpad()
{
	if (SDL_JoystickNumHats(joystick) > 0)
	{
		dpadState = SDL_JoystickGetHat(joystick, 0); // Assuming 0 is the D-Pad hat index
	}
}

void Controller::pollControllerState()
{
	// Poll axis values
	for (int i = 0; i < SDL_JoystickNumAxes(joystick); ++i)
	{
		axisValues[i] = SDL_JoystickGetAxis(joystick, i);
	}

	// Poll button states
	for (int i = 0; i < SDL_JoystickNumButtons(joystick); ++i)
	{
		buttonStates[i] = SDL_JoystickGetButton(joystick, i);
	}
}

float Controller::normalizeAxis(int aAxisValue, int aMaxValue)
{
	// Normalize the input value to a float in the range [-1, 1]
	if (aAxisValue >= 0)
	{
		return static_cast<float>(aAxisValue) / aMaxValue;
	}
	else
	{
		return static_cast<float>(aAxisValue) / -aMaxValue;
	}
}

int Controller::getAxis(int axis) const
{
	if (axis < 0 || axis >= static_cast<int>(axisValues.size()))
	{
		std::cerr << "Axis index out of range" << std::endl;
		return 0;
	}
	return axisValues[axis];
}

int Controller::readDpad()
{
	switch (dpadState)
	{
	case SDL_HAT_UP:
		std::cout << "D-Pad: UP" << std::endl;
		return DPadUp;
		break;
	case SDL_HAT_DOWN:
		std::cout << "D-Pad: DOWN" << std::endl;
		return DPadDown;
		break;
	case SDL_HAT_LEFT:
		std::cout << "D-Pad: LEFT" << std::endl;
		return DPadLeft;
		break;
	case SDL_HAT_RIGHT:
		std::cout << "D-Pad: RIGHT" << std::endl;
		return DPadRight;
		break;
	case SDL_HAT_CENTERED:
		std::cout << "D-Pad: CENTERED" << std::endl;
		return DPadNothing;
		break;
	default:
		return DPadNothing;
	}
}

void Controller::print()
{
	for (int i = 0; i < buttonStates.size(); i++)
	{
		std::cout << "Button " << i << " is " << buttonStates[i] << std::endl;
	}

	for (int i = 0; i < axisValues.size(); i++)
	{
		std::cout << "Axis " << i << " is " << axisValues[i] << std::endl;
	}
}

bool Controller::isButtonPressed(Key aKey)
{
	static std::map<int, int> myMap = {{(int)Key::Controller_Cross, 0},		 {(int)Key::Controller_Circle, 1},
									   {(int)Key::Controller_Square, 2},	 {(int)Key::Controller_Triangle, 3},
									   {(int)Key::Controller_L1, 9},		 {(int)Key::Controller_R1, 10},
									   {(int)Key::Controller_L3, 7},		 {(int)Key::Controller_R3, 8},
									   {(int)Key::Controller_DPad_Up, 11},	 {(int)Key::Controller_DPad_Down, 12},
									   {(int)Key::Controller_DPad_Left, 13}, {(int)Key::Controller_DPad_Right, 14}};

	int button = myMap[(int)aKey];
	if (buttonStates[button] == 1 && previousButtonStates[button] == 0)
	{
		return true;
	}
	return false;
}

bool Controller::isButton(Key aKey)
{
	static std::map<int, int> myMap = {{(int)Key::Controller_Cross, 0},		 {(int)Key::Controller_Circle, 1},
									   {(int)Key::Controller_Square, 2},	 {(int)Key::Controller_Triangle, 3},
									   {(int)Key::Controller_L1, 9},		 {(int)Key::Controller_R1, 10},
									   {(int)Key::Controller_L3, 7},		 {(int)Key::Controller_R3, 8},
									   {(int)Key::Controller_DPad_Up, 11},	 {(int)Key::Controller_DPad_Down, 12},
									   {(int)Key::Controller_DPad_Left, 13}, {(int)Key::Controller_DPad_Right, 14}};

	int button = myMap[(int)aKey];
	return buttonStates[button];
}

bool Controller::checkDeadzone(int aAxisValue)
{
	if (abs(aAxisValue) < 6000)
	{
		return true;
	}
	return false;
}

float Controller::getLeftStickUp()
{
	int axisValue = getAxis(LEFT_STICK_UP_AXIS);

	if (axisValue > 0)
	{
		return 0.0f;
	}

	if (checkDeadzone(axisValue))
	{
		return 0.0f;
	}

	float normalizedValue = normalizeAxis(abs(axisValue), pow(2, 15));

	return normalizedValue;
}

float Controller::getLeftStickLeft()
{
	int axisValue = getAxis(LEFT_STICK_LEFT_AXIS);

	if (axisValue > 0)
	{
		return 0.0f;
	}

	if (checkDeadzone(axisValue))
	{
		return 0.0f;
	}

	float normalizedValue = normalizeAxis(abs(axisValue), pow(2, 15));

	return normalizedValue;
}

float Controller::getLeftStickDown()
{
	int axisValue = getAxis(LEFT_STICK_DOWN_AXIS);

	if (axisValue < 0)
	{
		return 0.0f;
	}

	if (checkDeadzone(axisValue))
	{
		return 0.0f;
	}

	float normalizedValue = normalizeAxis(abs(axisValue), pow(2, 15));

	return normalizedValue;
}

float Controller::getLeftStickRight()
{
	int axisValue = getAxis(LEFT_STICK_RIGHT_AXIS);

	if (axisValue < 0)
	{
		return 0.0f;
	}

	if (checkDeadzone(axisValue))
	{
		return 0.0f;
	}

	float normalizedValue = normalizeAxis(abs(axisValue), pow(2, 15));

	return normalizedValue;
}

float Controller::getRightStickUp()
{
	int axisValue = getAxis(RIGHT_STICK_UP_AXIS);

	if (axisValue > 0)
	{
		return 0.0f;
	}

	if (checkDeadzone(axisValue))
	{
		return 0.0f;
	}

	float normalizedValue = normalizeAxis(abs(axisValue), pow(2, 15));

	return normalizedValue;
}

float Controller::getRightStickLeft()
{
	int axisValue = getAxis(RIGHT_STICK_LEFT_AXIS);

	if (axisValue > 0)
	{
		return 0.0f;
	}

	if (checkDeadzone(axisValue))
	{
		return 0.0f;
	}

	float normalizedValue = normalizeAxis(abs(axisValue), pow(2, 15));

	return normalizedValue;
}

float Controller::getRightStickDown()
{
	int axisValue = getAxis(RIGHT_STICK_DOWN_AXIS);

	if (axisValue < 0)
	{
		return 0.0f;
	}

	if (checkDeadzone(axisValue))
	{
		return 0.0f;
	}

	float normalizedValue = normalizeAxis(abs(axisValue), pow(2, 15));

	return normalizedValue;
}

float Controller::getRightStickRight()
{
	int axisValue = getAxis(RIGHT_STICK_RIGHT_AXIS);

	if (axisValue < 0)
	{
		return 0.0f;
	}

	if (checkDeadzone(axisValue))
	{
		return 0.0f;
	}

	float normalizedValue = normalizeAxis(abs(axisValue), pow(2, 15));

	return normalizedValue;
}
