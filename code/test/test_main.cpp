#include "Components/Component.h"
#include <gtest/gtest.h>

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(ExampleSuite, ExampleTest) {
    ASSERT_EQ(1, 1);
    ASSERT_NE(1, 0);
}