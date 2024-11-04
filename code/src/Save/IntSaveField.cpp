#include "IntSaveField.h"

IntSaveField::IntSaveField(std::string aName) : mName(aName) {}

IntSaveField::IntSaveField(std::string aName, int aValue) : mName(aName), mValue(aValue) {}

std::string IntSaveField::getName() const { return mName; }

void IntSaveField::setValue(int aValue) { mValue = aValue; }

int IntSaveField::getValue() const { return mValue; }