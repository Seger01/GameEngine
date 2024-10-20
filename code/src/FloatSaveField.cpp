#include "FloatSaveField.hpp"

FloatSaveField::FloatSaveField(std::string name) : mName(name) {}

FloatSaveField::FloatSaveField(std::string name, float value)
    : mName(name), mValue(value) {}

std::string FloatSaveField::getName() const { return mName; }

void FloatSaveField::setValue(float aValue) { mValue = aValue; }

float FloatSaveField::getValue() const { return mValue; }