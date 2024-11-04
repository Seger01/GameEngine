#pragma once

#include <string>

class StringSaveField {
public:
    StringSaveField(std::string aName);
    StringSaveField(std::string aName, std::string aValue);

    std::string getName() const;

    void setValue(std::string aValue);

    std::string getValue() const;

private:
    std::string mName;
    std::string mValue;
};