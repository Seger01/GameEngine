#include "IntSaveField.hpp"

IntSaveField::IntSaveField(std::string name) : mName(name) {}

std::string IntSaveField::getName() const { return mName; }

void IntSaveField::setValue(int aValue) { mValue = aValue; }

int IntSaveField::getValue() const { return mValue; }