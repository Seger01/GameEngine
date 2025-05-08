#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <SDL2/SDL.h>
#include <vector>

#include "Input/InputStructs.h"

class Controller
{
public:
	Controller(int index);
	~Controller();

	bool isConnected() const;
	void update();

	void print();

	int getAxis(int axis) const;
	bool isButtonPressed(Key aKey);
	bool isButton(Key aKey);

	float getLeftStickUp();
	float getLeftStickLeft();
	float getLeftStickDown();
	float getLeftStickRight();

	float getRightStickUp();
	float getRightStickLeft();
	float getRightStickDown();
	float getRightStickRight();

private:
	SDL_Joystick* joystick;
	int joystickIndex;
	bool connected;
	std::vector<int> axisValues;
	std::vector<bool> buttonStates;
	std::vector<bool> previousButtonStates;

	void pollControllerState();
	void pollDpad();
	float normalizeAxis(int aAxisValue, int aMaxValue);
	bool checkDeadzone(int aAxisValue);
	int readDpad();

	Uint8 dpadState; // For storing the current D-Pad state
};

#endif // CONTROLLER_H
