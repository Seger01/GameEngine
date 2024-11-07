#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "EngineBravo.h"

void engineTest() {
    GameObject defaultPlayerPrefab;
    defaultPlayerPrefab.setName("Player");
    EngineBravo& engine = EngineBravo::getInstance();
    SceneManager& sceneManager = engine.getSceneManager();
    NetworkManager& networkManager = engine.getNetworkManager();

    networkManager.setDefaultPlayerPrefab(defaultPlayerPrefab);

    std::string role;
    std::cout << "Do you want to be a server or a client? ";
    std::cin >> role;
    bool roleSet = false;

    while (!roleSet) {
        if (role == "server") {
            roleSet = true;
            networkManager.startServer();
        } else if (role == "client") {
            roleSet = true;
            networkManager.startClient();
        } else {
            std::cerr << "Invalid input. Please enter 'server' or 'client'." << std::endl;
            std::cin >> role;
        }
    }

    Scene* scene = sceneManager.createScene("Level1");
    if (scene == nullptr)
        exit(1);

    GameObject* gameObject = new GameObject;

    Transform objectTransform;

    objectTransform.position.x = 400;
    objectTransform.position.y = 400;

    gameObject->setTransform(objectTransform);

    // gameObject->addComponent<PlayerBehaviourScript>();

    scene->addGameObject(gameObject);

    sceneManager.loadScene(0);

    engine.initialize();

    if (role == "client") {
        std::vector<std::string> serverAddresses;
        networkManager.getClient().discoverServers(2000);
        while (!networkManager.getClient().getServerAddresses(serverAddresses)) {
            std::cout << "Searching for servers..." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        if (serverAddresses.empty()) {
            std::cerr << "No servers found" << std::endl;
            return;
        }

        std::cout << "Discovered servers:" << std::endl;
        for (const auto& address : serverAddresses) {
            std::cout << address << std::endl;
        }

        std::string serverAddress;
        std::cout << "Enter the IP address of the server you want to connect to: ";
        std::cin >> serverAddress;

        networkManager.getClient().setServerAddress(serverAddress);
        networkManager.getClient().connectToServer();
    }

    engine.run();

    return;
}

int main() {
    engineTest();

    return 0;
}
