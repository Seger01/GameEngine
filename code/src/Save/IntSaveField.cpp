#include "Save/IntSaveField.h"

/**
 * @brief Constructor to initialize the IntSaveField with a name and value.
 * @param aName The name associated with the int value.
 * @param aValue The initial int value.
 */
IntSaveField::IntSaveField(const std::string& aName, int aValue) : mName(aName), mValue(aValue) {}

/**
 * @brief Gets the name associated with the int value.
 * @return The name as a string.
 */
std::string IntSaveField::getName() const { return mName; }

/**
 * @brief Sets the int value.
 * @param aValue The new int value.
 */
void IntSaveField::setValue(int aValue) { mValue = aValue; }

/**
 * @brief Gets the current int value.
 * @return The current int value.
 */
int IntSaveField::getValue() const { return mValue; }
