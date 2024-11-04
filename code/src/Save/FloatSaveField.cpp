#include "FloatSaveField.h"

FloatSaveField::FloatSaveField(std::string aName) : mName(aName) {}

FloatSaveField::FloatSaveField(std::string aName, float aValue) : mName(aName), mValue(aValue) {}

std::string FloatSaveField::getName() const { return mName; }

void FloatSaveField::setValue(float aValue) { mValue = aValue; }

float FloatSaveField::getValue() const { return mValue; }