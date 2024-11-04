#pragma once

#include "FloatSaveField.h"
#include "IntSaveField.h"
#include "SaveArray.h"
#include "StringSaveField.h"
#include <string>

class SaveGame {
public:
    SaveGame(const std::string& aFileName);
    void store();
    void remove();

public:
    void addIntField(std::string name, int value);
    void addFloatField(std::string name, float value);
    void addStringField(std::string name, std::string value);

public:
    void setIntField(std::string aName, int aValue);
    void setFloatField(std::string aName, float aValue);
    void setStringField(std::string aName, std::string aValue);

public:
    bool hasIntField(std::string aName) const;
    bool hasFloatField(std::string aName) const;
    bool hasStringField(std::string aName) const;

public:
    const IntSaveField& getIntField(std::string aName) const;
    const FloatSaveField& getFloatField(std::string aName) const;
    const StringSaveField& getStringField(std::string aName) const;

public:
    void addArray(std::string name);
    void setArray(std::string aName, SaveArray aValue);
    SaveArray getArray(std::string aName) const;

private:
    void addAny(const std::string& aName, const nlohmann::json& aValue);
    void createFile();

private:
    std::string mFileName;
    std::vector<IntSaveField> mIntFields;
    std::vector<FloatSaveField> mFloatFields;
    std::vector<StringSaveField> mStringFields;
    std::vector<SaveArray> mArrays;
};