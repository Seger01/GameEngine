#include "NetworkBehaviourScript.h"

#include "InitBehaviourScript.h"
#include "NetworkButtonScript.h"

#include "Button.h"
#include "Engine/EngineBravo.h"
#include "Network/NetworkClient.h"
#include "Network/NetworkManager.h"
#include "Text.h"

void NetworkBehaviourScript::onStart() {
    EngineBravo& engine = EngineBravo::getInstance();
    SceneManager& sceneManager = engine.getSceneManager();
    Scene* scene = sceneManager.getCurrentScene();

    mServerButton = new Button;
    mServerButton->setTransform(Transform(Vector2(0, 0)));
    mServerButton->addComponent<NetworkButtonScript>();
    mServerButton->setWidth(40);
    mServerButton->setHeight(10);
    mServerButton->setTag("Server");
    Text* serverText = new Text("Server", "Server", Color(15, 110, 47), Vector2(0, 0), Vector2(0.5, 0.5));
    serverText->setParent(mServerButton);

    mClientButton = new Button;
    mClientButton->setTransform(Transform(Vector2(0, 20)));
    mClientButton->addComponent<NetworkButtonScript>();
    mClientButton->setWidth(40);
    mClientButton->setHeight(10);
    mClientButton->setTag("Client");
    Text* clientText = new Text("Client", "Client", Color(15, 110, 47), Vector2(0, 0), Vector2(0.5, 0.5));
    clientText->setParent(mClientButton);

    mHostButton = new Button;
    mHostButton->setTransform(Transform(Vector2(0, 40)));
    mHostButton->addComponent<NetworkButtonScript>();
    mHostButton->setWidth(40);
    mHostButton->setHeight(10);
    mHostButton->setTag("Host");
    Text* hostText = new Text("Host", "Host", Color(15, 110, 47), Vector2(0, 0), Vector2(0.5, 0.5));
    hostText->setParent(mHostButton);

    mSearchButton = new Button;
    mSearchButton->setTransform(Transform(Vector2(0, 0)));
    mSearchButton->addComponent<NetworkButtonScript>();
    mSearchButton->setWidth(40);
    mSearchButton->setHeight(10);
    mSearchButton->setTag("Search");
    mSearchButton->setInteractable(false);
    Text* connectText = new Text("Connect", "Connect", Color(15, 110, 47), Vector2(0, 0), Vector2(0.5, 0.5));
    connectText->setParent(mSearchButton);
    connectText->setActive(false);

    scene->addGameObject(mServerButton);
    scene->addGameObject(mClientButton);
    scene->addGameObject(mHostButton);
    scene->addGameObject(mSearchButton);

    scene->addGameObject(serverText);
    scene->addGameObject(clientText);
    scene->addGameObject(hostText);
    scene->addGameObject(connectText);
}

void NetworkBehaviourScript::onUpdate() {
    EngineBravo& engine = EngineBravo::getInstance();
    SceneManager& sceneManager = engine.getSceneManager();
    NetworkManager& networkManager = engine.getNetworkManager();

    if (networkManager.getRole() == NetworkRole::SERVER) {
        Scene* scene = sceneManager.createScene("tempScene");
        GameObject* tempObject = new GameObject;

        tempObject->addComponent<InitBehaviourScript>();

        scene->addGameObject(tempObject);

        int cameraID = scene->addCamera();
        scene->setActiveCamera(cameraID);

        scene->getActiveCamera().setTransform(Transform(Vector2(80, 96)));
        scene->getActiveCamera().setWidth(16 * 30);
        scene->getActiveCamera().setHeight(9 * 30);

        sceneManager.requestSceneChange("tempScene");
    }
    if (networkManager.getRole() == NetworkRole::CLIENT) {
        NetworkClient& networkClient = engine.getNetworkManager().getClient();
        std::vector<std::string> serverAddresses = networkClient.getServerAddresses();
        for (std::string serverAddress : serverAddresses) {
            auto it = std::find(mServerAddresses.begin(), mServerAddresses.end(), serverAddress);
            if (it == mServerAddresses.end()) {
                // Server address not found
                mServerAddresses.push_back(serverAddress);

                Button* ipButton = new Button;
                ipButton->setTransform(Transform(Vector2(100, mServerAddresses.size() * 20)));
                // ipButton->addComponent<NetworkButtonScript>();
                ipButton->setWidth(40);
                ipButton->setHeight(10);
                ipButton->setTag(serverAddress);
                Text* ipText =
                    new Text(serverAddress, serverAddress, Color(15, 110, 47), Vector2(0, 0), Vector2(0.5, 0.5));
                ipText->setParent(ipButton);

                Scene* scene = sceneManager.getCurrentScene();
                scene->addGameObject(ipButton);
                scene->addGameObject(ipText);
            }
        }
    }
}
