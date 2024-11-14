#include "Configuration.h"
#include "EngineBravo.h"
#include <gtest/gtest.h>

// Mock/friend class to access the Configuration singleton instance for testing.
class ConfigurationTest : public ::testing::Test {
protected:
    Configuration* config;

    void SetUp() override {
        // Assume EngineBravo has a static method to access the Configuration singleton for testing.
        config = &EngineBravo::getInstance().getConfiguration();
        config->clear();
    }
};

// Test setting and getting configuration values
TEST_F(ConfigurationTest, SetAndGetConfig) {
    config->setConfig("feature_enabled", true);
    EXPECT_TRUE(config->getConfig("feature_enabled"));

    config->setConfig("feature_enabled", false);
    EXPECT_FALSE(config->getConfig("feature_enabled"));
}

// Test getting a configuration key that hasn't been set
TEST_F(ConfigurationTest, GetUnsetConfigReturnsFalse) { EXPECT_FALSE(config->getConfig("nonexistent_key")); }

// Test setting multiple configuration values and retrieving them
TEST_F(ConfigurationTest, SetMultipleConfigs) {
    config->setConfig("feature_a", true);
    config->setConfig("feature_b", false);
    config->setConfig("feature_c", true);

    EXPECT_TRUE(config->getConfig("feature_a"));
    EXPECT_FALSE(config->getConfig("feature_b"));
    EXPECT_TRUE(config->getConfig("feature_c"));
}

// Test overwriting an existing configuration value
TEST_F(ConfigurationTest, OverwriteConfig) {
    config->setConfig("feature_toggle", true);
    EXPECT_TRUE(config->getConfig("feature_toggle"));

    config->setConfig("feature_toggle", false);
    EXPECT_FALSE(config->getConfig("feature_toggle"));
}

// Optional cleanup if Configuration has a reset or clear method
TEST_F(ConfigurationTest, ClearConfigs) {
    config->setConfig("feature_x", true);
    config->setConfig("feature_y", true);

    config->clear(); // Explicit clear for testing; assumes clear method is accessible
    EXPECT_FALSE(config->getConfig("feature_x"));
    EXPECT_FALSE(config->getConfig("feature_y"));
}
