#pragma once

#include <string>

class IntSaveField
{
public:
	IntSaveField(const std::string& aName, int aValue = 0);

	std::string getName() const;
	void setValue(int aValue);
	int getValue() const;

private:
	std::string mName;
	int mValue;
};