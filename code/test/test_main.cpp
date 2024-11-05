#include "FloatSaveField.h"
#include "IntSaveField.h"
#include "SaveGame.h"
#include "StringSaveField.h"
#include <cstdio>
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}