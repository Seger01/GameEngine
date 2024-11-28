#pragma once

#include <string>

class StringSaveField
{
public:
	StringSaveField(const std::string& aName, const std::string& aValue = "");

	std::string getName() const;
	void setValue(const std::string& aValue);
	std::string getValue() const;

private:
	std::string mName;
	std::string mValue;
};