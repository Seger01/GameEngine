#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <string>
#include <unordered_map>

/**
 * @class Configuration
 * @brief Singleton class to manage configuration settings.
 */
class Configuration {
public:
    friend class EngineBravo;

    /**
     * @brief Get the singleton instance of the Configuration class.
     * @return Reference to the singleton instance.
     */
    // Delete copy constructor and assignment operator to prevent copies
    Configuration(const Configuration&) = delete;
    Configuration& operator=(const Configuration&) = delete;

    /**
     * @brief Set a configuration value.
     * @param key The configuration key.
     * @param value The configuration value.
     */
    void setConfig(const std::string& key, bool value) { configs[key] = value; }

    /**
     * @brief Get a configuration value.
     * @param key The configuration key.
     * @return The configuration value. Returns false if the key is not found.
     */
    bool getConfig(const std::string& key) const {
        auto it = configs.find(key);
        if (it != configs.end()) {
            return it->second;
        }
        return false; // Default value if key not found
    }

private:
    /**
     * @brief Private constructor to prevent instantiation.
     */
    Configuration() {}

    std::unordered_map<std::string, bool> configs; ///< Map to store configuration key-value pairs.
};

#endif // CONFIGURATION_H
