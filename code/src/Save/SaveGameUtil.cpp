#include "Save/SaveGameUtil.h"

#include <iostream>
#include <sstream>

bool SaveGameUtil::isInteger(const std::string& aValue)
{
	std::istringstream iss(aValue);
	int val;
	// Attempt to read an integer from the string
	iss >> std::noskipws >> val;
	// Check if the entire string was used (iss should be fully consumed)
	return iss.eof() && !iss.fail();
}

bool SaveGameUtil::isFloat(const std::string& aValue)
{
	std::istringstream iss(aValue);
	float val;
	// Attempt to read a float from the string
	iss >> std::noskipws >> val;
	// Check if the entire string was used (iss should be fully consumed)
	return iss.eof() && !iss.fail();
}
