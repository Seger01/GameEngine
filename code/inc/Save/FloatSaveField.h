#pragma once

#include <string>

class FloatSaveField
{
public:
	FloatSaveField(const std::string& aName, float aValue = 0);

	std::string getName() const;
	void setValue(float aValue);
	float getValue() const;

private:
	std::string mName;
	float mValue;
};