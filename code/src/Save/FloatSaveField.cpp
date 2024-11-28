#include "FloatSaveField.h"

FloatSaveField::FloatSaveField(const std::string& aName, float aValue) : mName(aName), mValue(aValue) {}

std::string FloatSaveField::getName() const { return mName; }

void FloatSaveField::setValue(float aValue) { mValue = aValue; }

float FloatSaveField::getValue() const { return mValue; }