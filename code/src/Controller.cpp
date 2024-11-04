#include <iostream>
#include <map>

#include "Controller.h"

const int LEFT_STICK_UP_AXIS = 1;
const int LEFT_STICK_LEFT_AXIS = 0;
const int LEFT_STICK_DOWN_AXIS = 1;
const int LEFT_STICK_RIGHT_AXIS = 0;

const int RIGHT_STICK_UP_AXIS = 4;
const int RIGHT_STICK_LEFT_AXIS = 3;
const int RIGHT_STICK_DOWN_AXIS = 4;
const int RIGHT_STICK_RIGHT_AXIS = 3;

const int DPadNothing = -1;
const int DPadUp = 0;
const int DPadLeft = 1;
const int DPadDown = 2;
const int DPadRight = 3;

Controller::Controller(int index) : joystickIndex(index), joystick(nullptr), connected(false) {
    if (SDL_NumJoysticks() > joystickIndex) {
        joystick = SDL_JoystickOpen(joystickIndex);
        if (joystick) {
            connected = true;
            axisValues.resize(SDL_JoystickNumAxes(joystick), 0);
            buttonStates.resize(SDL_JoystickNumButtons(joystick), false);
        } else {
            std::cerr << "Failed to open joystick: " << SDL_GetError() << std::endl;
        }
    } else {
        std::cerr << "No joystick connected at index " << joystickIndex << std::endl;
    }
}

Controller::~Controller() {
    if (joystick) {
        SDL_JoystickClose(joystick);
    }
}

bool Controller::isConnected() const { return connected; }

void Controller::update() {
    if (!connected)
        return;

    SDL_JoystickUpdate();
    pollControllerState();
}

void Controller::pollDpad() {
    if (SDL_JoystickNumHats(joystick) > 0) {
        dpadState = SDL_JoystickGetHat(joystick, 0); // Assuming 0 is the D-Pad hat index
    }
}

void Controller::pollControllerState() {
    // Poll axis values
    for (int i = 0; i < SDL_JoystickNumAxes(joystick); ++i) {
        axisValues[i] = SDL_JoystickGetAxis(joystick, i);
    }

    // Poll button states
    for (int i = 0; i < SDL_JoystickNumButtons(joystick); ++i) {
        buttonStates[i] = SDL_JoystickGetButton(joystick, i);
    }
}

float Controller::normalizeAxis(int aAxisValue, int aMaxValue) {
    // Normalize the input value to a float in the range [-1, 1]
    if (aAxisValue >= 0) {
        return static_cast<float>(aAxisValue) / aMaxValue;
    } else {
        return static_cast<float>(aAxisValue) / -aMaxValue;
    }
}
int Controller::getAxis(int axis) const {
    if (axis < 0 || axis >= static_cast<int>(axisValues.size())) {
        std::cerr << "Axis index out of range" << std::endl;
        return 0;
    }
    return axisValues[axis];
}

int Controller::readDpad() {
    switch (dpadState) {
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

bool Controller::isButtonPressed(Key aKey) {
    std::map<int, int> myMap = {
        {(int)Key::Controller_Cross, 100}, {6, 200}, {7, 200}, {8, 200}, {2, 200}, {3, 300}, {4, 400}};

    int button = (int)aKey;
    if (button < 0 || button >= static_cast<int>(buttonStates.size())) {
        std::cerr << "Button index out of range" << std::endl;
        return false;
    }
    return buttonStates[button];
}

bool Controller::checkDeadzone(int aAxisValue) {
    if (abs(aAxisValue) < 6000) {
        return true;
    }
    return false;
}

float Controller::getLeftStickUp() {
    int axisValue = getAxis(LEFT_STICK_UP_AXIS);

    if (axisValue > 0) {
        return 0.0f;
    }

    if (checkDeadzone(axisValue)) {
        return 0.0f;
    }

    float normalizedValue = normalizeAxis(abs(axisValue), pow(2, 15));

    return normalizedValue;
}

float Controller::getLeftStickLeft() {
    int axisValue = getAxis(LEFT_STICK_LEFT_AXIS);

    if (axisValue > 0) {
        return 0.0f;
    }

    if (checkDeadzone(axisValue)) {
        return 0.0f;
    }

    float normalizedValue = normalizeAxis(abs(axisValue), pow(2, 15));

    return normalizedValue;
}

float Controller::getLeftStickDown() {
    int axisValue = getAxis(LEFT_STICK_DOWN_AXIS);

    if (axisValue < 0) {
        return 0.0f;
    }

    if (checkDeadzone(axisValue)) {
        return 0.0f;
    }

    float normalizedValue = normalizeAxis(abs(axisValue), pow(2, 15));

    return normalizedValue;
}

float Controller::getLeftStickRight() {
    int axisValue = getAxis(LEFT_STICK_RIGHT_AXIS);

    if (axisValue < 0) {
        return 0.0f;
    }

    if (checkDeadzone(axisValue)) {
        return 0.0f;
    }

    float normalizedValue = normalizeAxis(abs(axisValue), pow(2, 15));

    return normalizedValue;
}

float Controller::getRightStickUp() {
    int axisValue = getAxis(RIGHT_STICK_UP_AXIS);

    if (axisValue > 0) {
        return 0.0f;
    }

    if (checkDeadzone(axisValue)) {
        return 0.0f;
    }

    float normalizedValue = normalizeAxis(abs(axisValue), pow(2, 15));

    return normalizedValue;
}

float Controller::getRightStickLeft() {
    int axisValue = getAxis(RIGHT_STICK_LEFT_AXIS);

    if (axisValue > 0) {
        return 0.0f;
    }

    if (checkDeadzone(axisValue)) {
        return 0.0f;
    }

    float normalizedValue = normalizeAxis(abs(axisValue), pow(2, 15));

    return normalizedValue;
}

float Controller::getRightStickDown() {
    int axisValue = getAxis(RIGHT_STICK_DOWN_AXIS);

    if (axisValue < 0) {
        return 0.0f;
    }

    if (checkDeadzone(axisValue)) {
        return 0.0f;
    }

    float normalizedValue = normalizeAxis(abs(axisValue), pow(2, 15));

    return normalizedValue;
}

float Controller::getRightStickRight() {
    int axisValue = getAxis(RIGHT_STICK_RIGHT_AXIS);

    if (axisValue < 0) {
        return 0.0f;
    }

    if (checkDeadzone(axisValue)) {
        return 0.0f;
    }

    float normalizedValue = normalizeAxis(abs(axisValue), pow(2, 15));

    return normalizedValue;
}
