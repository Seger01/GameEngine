#pragma once

#include <string>

class IntSaveField {
public:
    IntSaveField(std::string aName);
    IntSaveField(std::string aName, int aValue);

    std::string getName() const;

    void setValue(int aValue);

    int getValue() const;

private:
    std::string mName;
    int mValue;
};