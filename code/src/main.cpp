#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_render.h>
#include <SDL_video.h>

#include "Animation.h"
#include "Rectangle.h"
#include "Renderer.h"
#include "SpriteAtlas.h"
#include "Window.h"

#include "Event.h"
#include "Inputs.h"

const int MOVE_SPEED = 10;

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

    // Subscribe function to add callbacks
    void subscribe(const std::function<void(float)>& callback) { callbacks.push_back(callback); }

    bool isInKeyList(Key aPotentialKey) {
        for (int i = 0; i < mTriggerKeys.size(); i++) {
            if (mTriggerKeys[i] == aPotentialKey) {
                return true;
            }
        }

        return false;
    }

    // Function to simulate an update and notify all subscribers
    void trigger(Key aReceivedKey) {
        if (!isInKeyList(aReceivedKey))
            return;

        for (const auto& callback : callbacks) {
            callback(1.0);
        }
    }

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

    // Vectoj to store all subscribed functions
    std::vector<std::function<void(float)>> callbacks;
};

class Context {
public:
    Context(std::string aContextName) {
        mContextName = aContextName;
        return;
    }

    std::string getName() { return mContextName; }

    void addDefAction(DefinedAction& aDefinedAction) {
        std::cout << "mRegisteredActions.push_back(aDefinedAction);" << std::endl;
        mRegisteredActions.push_back(aDefinedAction);

        return;
    }

    void processKey(Key aPressedKey) {
        for (int i = 0; i < mRegisteredActions.size(); i++) {
            mRegisteredActions[i].get().trigger(aPressedKey);
        }

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

    void subscribeToAction(DefAction aDefAction, const std::function<void(float)>& callback) {
        for (int i = 0; i < mDefActions.size(); i++) {
            if (mDefActions[i]->getID() == aDefAction) {
                mDefActions[i]->subscribe(callback);
            }
        }
    }

    void processKey(Key aInputKey) {
        mContexts[mActiveContextIndex].processKey(aInputKey);

        return;
    }

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

    void readKeys(std::string aActionName) {
        try {
            // Load key mappings from the JSON file

            // Using operator[] to retrieve the vector of strings associated with the key
            // std::vector<std::string> values = defActions[aActionName]; // Directly access the vector
            //
            // for (int i = 0; i < values.size(); i++) {
            //     mTriggerKeys.push_back(static_cast<Key>(stringToKeyID(values[i])));
            // }

        } catch (const std::exception& ex) {
            std::cerr << "Error: " << ex.what() << std::endl;
        }
    }

private:
    int mActiveContextIndex = 0;

    std::vector<std::shared_ptr<DefinedAction>> mDefActions;

    std::vector<Context> mContexts;
};

class Input {
public:
    Input() {}

    void subscribe(DefAction aDefAction, const std::function<void(float)>& callback) {
        mContextManager.subscribeToAction(aDefAction, callback);
    }

    void update() {
        int numKeys;
        const Uint8* allKeys = SDL_GetKeyboardState(&numKeys);

        for (int i = 0; i < numKeys; ++i) {
            if (allKeys[i]) {
                // std::cout << "Key with scancode " << keyToString((Key)i) << " is pressed" << std::endl;
                mContextManager.processKey((Key)i);
            }
        }

        return;
    }

    bool getKeyDown(Key aKeyToCheck) { return false; }
    bool isKeyDown() { return false; }

private:
    ContextManager mContextManager;

    Uint8* mAllKeys = nullptr;
};

void onMouseDownEvent(const Event& aEvent) {
    if (aEvent.mouse.left) {
        std::cout << "mouse left pressed at " << aEvent.mouse.x << ", " << aEvent.mouse.y << std::endl;
    }
    if (aEvent.mouse.middle) {
        std::cout << "mouse middle pressed at " << aEvent.mouse.x << ", " << aEvent.mouse.y << std::endl;
    }
    if (aEvent.mouse.right) {
        std::cout << "mouse right pressed at " << aEvent.mouse.x << ", " << aEvent.mouse.y << std::endl;
    }
}

bool quit = false;

void enditall(const Event& event) { quit = true; }

void anyEvent(const Event& aEvent) {
    std::cout << "this will trigger with every event!!!" << std::endl;

    return;
}

void handlePlayerMovement(const Event& aEvent) {
    std::cout << "Player movement detected" << std::endl;

    return;
}

void run() {
    // contextManager.setActiveContext("Playing");
    EventManager eventManager;
    eventManager.subscribe(onMouseDownEvent, EventType::MouseButtonDown);
    eventManager.subscribe(enditall, EventType::Quit);
    eventManager.subscribe(handlePlayerMovement, EventType::DefinedAction);
    eventManager.subscribe(anyEvent);

    Window myWindow;
    Renderer* myRenderer = new Renderer(myWindow);

    SDL_Event event;

    // Variables for animation timing and sprite movement
    Uint32 lastTime = 0;         // Time of the last frame change
    int frame = 0;               // Current frame (sprite in the sprite sheet)
    const int spriteWidth = 16;  // Width of each sprite
    const int spriteHeight = 25; // Height of each sprite
    const int frameCount = 6;    // Total number of frames in the sprite sheet

    // SDL_Texture* spriteSheetTexture = loadTexture(renderer, "enter_the_gungeon_spritesheet.png");
    SpriteAtlas spriteAtlas(myRenderer, "enter_the_gungeon_spritesheet.png");

    Rectangle startOfAnimation;
    startOfAnimation.x = 22;          // Move horizontally in the sprite sheet
    startOfAnimation.y = 187;         // Keep the vertical position constant (you can change this for vertical movement)
    startOfAnimation.w = spriteWidth; // The width of the sprite
    startOfAnimation.h = spriteHeight; // The height of the sprite

    Animation& animation = spriteAtlas.getAnimation(startOfAnimation, frameCount);

    Rectangle destRect;
    // Define the destination rect where the image will be drawn
    destRect.x = 100;    // The x position on the screen
    destRect.y = 100;    // The y position on the screen
    destRect.w = 18 * 4; // The width of the drawn image (scaling)
    destRect.h = 26 * 4; // The height of the drawn image (scaling)

    while (!quit) {
        eventManager.handleEvents();

        // Clear screen
        // SDL_RenderClear(renderer);
        myRenderer->clear();

        // Texture* spriteTexture = animation.getTexture();

        // Copy part of the sprite sheet to the renderer
        // SDL_RenderCopy(renderer, spriteTexture->getSDL_Texture(), &srcRect, &destRect);
        myRenderer->renderAnimation(animation, destRect);

        // Update the screen
        // SDL_RenderPresent(renderer);
        myRenderer->present();

        SDL_Delay(16);
    }
    // Clean up
    // deInitSDL(window, renderer);
    return;
}
int main() {
    run();

    // std::vector<Key> keys = {Key::Key_W, Key::Key_A, Key::Key_S};
    //
    // DefinedAction action(0, keys);
    //
    // action.print();

    // // Subscribe external functions
    // context.subscribe(externalFunction1);
    // context.subscribe(externalFunction2);
    //
    // // Subscribe a lambda function for variety
    // context.subscribe([](float value) { std::cout << "Lambda function received value: " << value << std::endl;
    // });
    //
    // // Simulate an update
    // context.update(42.0f);

    // for (int i = 0; i < action.mTriggerKeys.size(); i++) {
    //     std::cout << static_cast<int>(action.mTriggerKeys[i]) << std::endl;
    // }

    return 0;
}
