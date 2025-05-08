#include "Save/StringSaveField.h"

/**
 * @brief Constructor to initialize the StringSaveField with a name and value.
 * @param aName The name associated with the string value.
 * @param aValue The initial string value.
 */
StringSaveField::StringSaveField(const std::string& aName, const std::string& aValue) : mName(aName), mValue(aValue) {}

/**
 * @brief Gets the name associated with the string value.
 * @return The name as a string.
 */
std::string StringSaveField::getName() const { return mName; }

/**
 * @brief Sets the string value.
 * @param aValue The new string value.
 */
void StringSaveField::setValue(const std::string& aValue) { mValue = aValue; }

/**
 * @brief Gets the current string value.
 * @return The current string value.
 */
std::string StringSaveField::getValue() const { return mValue; }
