#include "Save/FloatSaveField.h"

/**
 * @brief Constructor to initialize the FloatSaveField with a name and value.
 * @param aName The name associated with the float value.
 * @param aValue The initial float value.
 */
FloatSaveField::FloatSaveField(const std::string& aName, float aValue) : mName(aName), mValue(aValue) {}

/**
 * @brief Gets the name associated with the float value.
 * @return The name as a string.
 */
std::string FloatSaveField::getName() const { return mName; }

/**
 * @brief Sets the float value.
 * @param aValue The new float value.
 */
void FloatSaveField::setValue(float aValue) { mValue = aValue; }

/**
 * @brief Gets the current float value.
 * @return The current float value.
 */
float FloatSaveField::getValue() const { return mValue; }
