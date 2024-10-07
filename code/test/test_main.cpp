#include <gtest/gtest.h>
#include "test.h"

TEST(ExampleSuite, ExampleTest) {
    ASSERT_EQ(1,1);
    ASSERT_NE(1,0);
    ASSERT_EQ(1,returnsOne());
}