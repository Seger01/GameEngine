#include "NetworkButtonScript.h"
#include "Button.h"
#include "Engine/EngineBravo.h"
#include "Network/NetworkManager.h"
#include "Text.h"

#include <iostream>

NetworkButtonScript::NetworkButtonScript() : mRoleSelect(true) {}

NetworkButtonScript::~NetworkButtonScript() {}

void NetworkButtonScript::onButtonPressed() {
    Button* button = dynamic_cast<Button*>(mGameObject);
    if (button) {
        EngineBravo& engine = EngineBravo::getInstance();
        NetworkManager& networkManager = engine.getNetworkManager();

        if (button->getTag() == "Server") {
            std::cout << "Server selected" << std::endl;
            if (networkManager.isConnected()) {
                std::cout << "Network is already connected" << std::endl;
            } else {
                networkManager.setRole(NetworkRole::SERVER);
                NetworkManager.startNetwork();
            }
            setButtonsVisibility();

        } else if (button->getTag() == "Client") {
            std::cout << "Client selected" << std::endl;
            if (networkManager.isConnected()) {
                std::cout << "Network is already connected" << std::endl;
            } else {
                networkManager.setRole(NetworkRole::CLIENT);
                NetworkManager.startNetwork();
            }
            setButtonsVisibility();
        } else if (button->getTag() == "Host") {
            std::cout << "Host selected" << std::endl;
            if (networkManager.isConnected()) {
                std::cout << "Network is already connected" << std::endl;
            } else {
                networkManager.setRole(NetworkRole::HOST);
            }
            setButtonsVisibility();
        } else if (button->getTag() == "Search") {
            std::cout << "Search selected" << std::endl;
            NetworkClient* networkClient = engine.getNetworkManager().getNetworkClient();
            if (networkClient) {
                networkClient->discoverServers();
            } else {
                std::cout << "NetworkClient is nullptr" << std::endl;
            }
        } else {
            std::cout << "Unknown tag" << button->getTag() << std::endl;
        }

    } else {
        std::cout << "Parent object not a button" << std::endl;
    }
}

void NetworkButtonScript::onButtonReleased() {}

void NetworkButtonScript::setButtonsVisibility() {
    EngineBravo& engine = EngineBravo::getInstance();
    SceneManager& sceneManager = engine.getSceneManager();
    Scene* scene = sceneManager.getCurrentScene();

    for (auto& gameObject : scene->getGameObjects()) {
        Button* button = dynamic_cast<Button*>(gameObject);
        Text* text = dynamic_cast<Text*>(gameObject);
        if (button) {
            button->setActive(!button->interactable());
        }
        if (text) {
            text->setActive(!text->isActive());
        }
    }
}