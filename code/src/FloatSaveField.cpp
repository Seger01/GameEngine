#include "FloatSaveField.hpp"

FloatSaveField::FloatSaveField(std::string name) : mName(name) {}

std::string FloatSaveField::getName() const { return mName; }

void FloatSaveField::setValue(float aValue) { mValue = aValue; }

float FloatSaveField::getValue() const { return mValue; }