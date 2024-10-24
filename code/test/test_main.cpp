#include "FloatSaveField.hpp"
#include "IntSaveField.hpp"
#include "SaveGame.hpp"
#include "StringSaveField.hpp"
#include <cstdio>
#include <gtest/gtest.h>
#include <iostream>

TEST(SaveGame, ValidWriteRead) {
  std::string path = "code/output/newSave.json";
  std::remove(path.c_str());
  SaveGame sg{path};
  sg.addStringField("string1", "value1");
  sg.addIntField("int1", 1);
  sg.addFloatField("float1", 1.0f);

  ASSERT_FLOAT_EQ(sg.getFloatField("float1").getValue(), 1.0f);

  sg.setFloatField("float1", 1.1f);

  ASSERT_EQ(sg.getStringField("string1").getValue(), "value1");
  ASSERT_EQ(sg.getIntField("int1").getValue(), 1);
  ASSERT_FLOAT_EQ(sg.getFloatField("float1").getValue(), 1.1f);
}

TEST(SaveGame, InvalidWriteRead) {
  SaveGame sg{"code/output/newSave.json"};

  ASSERT_THROW(sg.getStringField("string1"), std::invalid_argument);
  ASSERT_THROW(sg.getIntField("int1"), std::invalid_argument);
  ASSERT_THROW(sg.getFloatField("float1"), std::invalid_argument);

  ASSERT_THROW(sg.setStringField("string1", "value1"), std::invalid_argument);
  ASSERT_THROW(sg.setIntField("int1", 1), std::invalid_argument);
  ASSERT_THROW(sg.setFloatField("float1", 1.0f), std::invalid_argument);

  ASSERT_THROW(sg.setArray("array1", SaveArray("array1name")),
               std::invalid_argument);
}

TEST(SaveGame, Store) {
  // Test SaveGame::store
}

TEST(SaveGame, Remove) {
  // Test SaveGame::remove
}

TEST(SaveGame, InvalidRemove) {}

TEST(SaveGame, ArrayWriteRead) {
  // Test SaveGame::addArray, SaveGame::setArray, SaveGame::getArray
}