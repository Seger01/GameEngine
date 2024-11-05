#include "FloatSaveField.h"
#include "IntSaveField.h"
#include "SaveGame.h"
#include "StringSaveField.h"
#include <cstdio>
#include <fstream>
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

    ASSERT_THROW(sg.setArray("array1", SaveArray("array1name")), std::invalid_argument);
}

TEST(SaveGame, Store) {
    std::string path{"code/output/newSave.json"};
    std::remove(path.c_str());
    SaveGame sg{path};
    sg.addIntField("int1", 1);
    sg.addFloatField("float1", 1.0f);
    sg.addArray("array1");
    SaveArray array{sg.getArray("array1")};
    array.addStringField("string1", "value1");
    sg.setArray("array1", array);
    sg.store();

    // Check if the file contents are correct
    std::ifstream file(path);
    ASSERT_TRUE(file.is_open());

    std::string fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::string expectedContent = "{\n"
                                  "    \"arrays\": [\n"
                                  "        {\n"
                                  "            \"fields\": [\n"
                                  "                {\n"
                                  "                    \"name\": \"string1\",\n"
                                  "                    \"value\": \"value1\"\n"
                                  "                }\n"
                                  "            ],\n"
                                  "            \"name\": \"array1\"\n"
                                  "        }\n"
                                  "    ],\n"
                                  "    \"fields\": [\n"
                                  "        {\n"
                                  "            \"name\": \"int1\",\n"
                                  "            \"value\": 1\n"
                                  "        },\n"
                                  "        {\n"
                                  "            \"name\": \"float1\",\n"
                                  "            \"value\": 1.0\n"
                                  "        }\n"
                                  "    ]\n"
                                  "}";

    file.close();

    ASSERT_EQ(expectedContent, fileContent);

    std::remove(path.c_str());
}

TEST(SaveGame, InvalidStore) {
    std::string invalidPath{"invalid/invalid.json"};
    std::remove(invalidPath.c_str());
    SaveGame sg{invalidPath};
    sg.addIntField("int1", 1);
    sg.addFloatField("float1", 1.0f);
    sg.addArray("array1");
    SaveArray array{sg.getArray("array1")};
    array.addStringField("string1", "value1");
    sg.setArray("array1", array);
    ASSERT_THROW(sg.store(), std::invalid_argument);

    // Check if the file contents are correct
    std::ifstream file(invalidPath);
    ASSERT_FALSE(file.is_open());

    std::remove(invalidPath.c_str());
}

TEST(SaveGame, Remove) {
    // Test SaveGame::remove
}

TEST(SaveGame, InvalidRemove) {}

TEST(SaveGame, ArrayWriteRead) {
    // Test SaveGame::addArray, SaveGame::setArray, SaveGame::getArray
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}