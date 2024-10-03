#pragma once

#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <SDL.h>
#include <nlohmann/json.hpp>

#include "Input.h"
#include "InputStructs.h"

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

    float isActive() {
        for (int i = 0; i < mTriggerKeys.size(); i++) {
            // if (Input::getInstance().)
            // if (Input::getInstance().GetKey(mTriggerKeys[i])) {
            //     return 1.0;
            // }
        }

        return 0;
    }

    bool isInKeyList(Key aPotentialKey) {
        for (int i = 0; i < mTriggerKeys.size(); i++) {
            if (mTriggerKeys[i] == aPotentialKey) {
                return true;
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
