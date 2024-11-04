#pragma once

#include <string>

class FloatSaveField {
public:
    FloatSaveField(std::string name);
    FloatSaveField(std::string name, float value);

    std::string getName() const;

    void setValue(float aValue);

    float getValue() const;

private:
    std::string mName;
    float mValue;
};