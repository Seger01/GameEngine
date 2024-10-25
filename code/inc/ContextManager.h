#ifndef CONTEXTMANAGER_H
#define CONTEXTMANAGER_H

#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <SDL.h>
#include <nlohmann/json.hpp>

#include "InputStructs.h"
//
// Alias for convenience
using json = nlohmann::json;

// Class definition
class DefinedAction {
public:
    DefinedAction(DefAction aActionID, std::vector<Key> aActions) {
        mTriggerKeys = aActions;
        mActionID = aActionID;
        return;
    }

    // // Subscribe function to add callbacks
    // void subscribe(const std::function<void(float)>& callback) { callbacks.push_back(callback); }

    bool isActive(std::vector<Key> aHeldKeys) {
        for (auto& heldKey : aHeldKeys) {
            for (auto& triggerKey : mTriggerKeys) {
                if (heldKey == triggerKey) {
                    return true;
                }
            }
        }

        return false;
    }

    // Function to simulate an update and notify all subscribers
    // void trigger(Key aReceivedKey) {
    //     if (!isInKeyList(aReceivedKey))
    //         return;
    //
    //     for (const auto& callback : callbacks) {
    //         callback(1.0);
    //     }
    // }

    void print() {
        std::cout << actionToString(mActionID) << std::endl;
        for (int i = 0; i < mTriggerKeys.size(); i++) {
            std::cout << "   " << keyToString(mTriggerKeys[i]) << std::endl;
        }
    }

    DefAction getID() { return mActionID; }

private:
    DefAction mActionID = DefAction::Undefined;

    std::vector<Key> mTriggerKeys = {};

    // // Vectoj to store all subscribed functions
    // std::vector<std::function<void(float)>> callbacks;
};

class Context {
public:
    Context(std::string aContextName) {
        mContextName = aContextName;
        return;
    }

    std::vector<DefAction> getCurrentActions(std::vector<Key> aHeldKeys) {
        std::vector<DefAction> activeActions;
        std::cout << "mRegisteredActions.size(): " << mRegisteredActions.size() << std::endl;
        for (auto& action : mRegisteredActions) {
            if (action.get().isActive(aHeldKeys)) {
                activeActions.push_back(action.get().getID());
            }
        }

        return activeActions;
    }

    std::string getName() { return mContextName; }

    void addDefAction(DefinedAction& aDefinedAction) {
        std::cout << "mRegisteredActions.push_back(aDefinedAction);" << std::endl;
        mRegisteredActions.push_back(aDefinedAction);

        return;
    }

    void print() {
        std::cout << "Context name: " << mContextName << " amount of registerd actions: " << mRegisteredActions.size()
                  << std::endl;

        for (auto& defAction : mRegisteredActions) {
            defAction.get().print();
        }
    }

private:
    std::vector<std::reference_wrapper<DefinedAction>> mRegisteredActions;

    std::string mContextName;
};

class ContextManager {
public:
    ContextManager() {
        this->loadDefinedActions();

        this->createContexts();

        return;
    }

    std::vector<DefAction> getCurrentActions(std::vector<Key> aHeldKeys) {
        std::cout << "Active context is: " << mContexts[this->mActiveContextIndex].getName() << std::endl;
        std::cout << "aHeldKeys.size(): " << aHeldKeys.size() << std::endl;
        return mContexts[this->mActiveContextIndex].getCurrentActions(aHeldKeys);
    }

    // void subscribeToAction(DefAction aDefAction, const std::function<void(float)>& callback) {
    //     for (int i = 0; i < mDefActions.size(); i++) {
    //         if (mDefActions[i]->getID() == aDefAction) {
    //             mDefActions[i]->subscribe(callback);
    //         }
    //     }
    // }

    // void processKey(Key aInputKey) {
    //     mContexts[mActiveContextIndex].processKey(aInputKey);
    //
    //     return;
    // }

    void setActiveContext(std::string aWantedContex) {
        for (int i = 0; i < mContexts.size(); i++) {
            if (mContexts[i].getName() == aWantedContex) {
                mActiveContextIndex = i;
                break;
            }

            if (i == mContexts.size() - 1) {
                std::cout << "Failed to find context: " << aWantedContex << std::endl;
            }
        }

        return;
    }

    void createContexts() {
        std::map<std::string, std::vector<std::string>> defActions;

        try {
            defActions = readJsonFile("contexts.json");
        } catch (const std::exception& ex) {
            std::cerr << "Error: " << ex.what() << std::endl;
        }

        for (const auto& pair : defActions) {
            const std::string& contextName = pair.first;
            const std::vector<std::string>& keys = pair.second;

            Context context(contextName);

            std::cout << "mDefActions.size() " << mDefActions.size() << std::endl;

            for (int iDefActions = 0; iDefActions < mDefActions.size(); iDefActions++) {
                for (int i = 0; i < keys.size(); i++) {
                    std::cout << "Comparing: " << actionToString(mDefActions[iDefActions]->getID()) << ", " << keys[i]
                              << std::endl;
                    if (actionToString(mDefActions[iDefActions]->getID()) == keys[i]) {
                        context.addDefAction(*mDefActions[iDefActions]);
                    }
                }
            }
            // context.addDefAction(this->mDefActions[0]);
            mContexts.push_back(context);
        }

        for (int i = 0; i < mContexts.size(); i++) {
            mContexts[i].print();
        }
    }

    void loadDefinedActions() {
        std::map<std::string, std::vector<std::string>> defActions;

        try {
            defActions = readJsonFile("defined_actions.json");
        } catch (const std::exception& ex) {
            std::cerr << "Error: " << ex.what() << std::endl;
        }

        // Iterate over all keys and values
        for (const auto& pair : defActions) {
            const std::string& definedActionName = pair.first;
            const std::vector<std::string>& keys = pair.second;

            std::vector<Key> keyIDs;

            for (int i = 0; i < keys.size(); i++) {
                keyIDs.push_back(static_cast<Key>(stringToKeyID(keys[i])));
            }

            mDefActions.push_back(
                std::make_shared<DefinedAction>(static_cast<DefAction>(stringToActionID(definedActionName)), keyIDs));
        }

        for (int i = 0; i < mDefActions.size(); i++) {
            mDefActions[i]->print();
        }
    }

    // Function to read JSON data into map
    std::map<std::string, std::vector<std::string>> readJsonFile(const std::string& filename) {
        // Create a map to store the data
        std::map<std::string, std::vector<std::string>> keyMap;

        // Open the JSON file
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file");
        }

        // Parse the JSON file
        json jsonData;
        file >> jsonData;

        // Check if the "KeyMappings" section exists

        // Iterate over each key-value pair in the "KeyMappings" section
        for (auto& [key, value] : jsonData.items()) {
            // Ensure that the value is an array of strings
            if (value.is_array()) {
                std::vector<std::string> strings;
                for (const auto& item : value) {
                    if (item.is_string()) {
                        strings.push_back(item);
                    }
                }
                // Insert the array of strings into the map
                keyMap[key] = strings;
            }
        }

        // Return the populated map
        return keyMap;
    }

private:
    int mActiveContextIndex = 0;

    std::vector<std::shared_ptr<DefinedAction>> mDefActions;

    std::vector<Context> mContexts;
};

#endif
