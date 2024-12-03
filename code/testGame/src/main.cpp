#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "EngineBravo.h"

#include "InitBehaviourScript.h"
#include "Network/NetworkTransform.h"
#include "PlayerPrefab.h"
#include "StartupBehaviourScript.h"

void engineTest()
{
	EngineBravo& engine = EngineBravo::getInstance();
	engine.initialize();
	SceneManager& sceneManager = engine.getSceneManager();

	NetworkManager& networkManager = engine.getNetworkManager();

	GameObject* defaultPlayerPrefab = PlayerPrefabFactory().createPlayerPrefab();

	networkManager.setDefaultPlayerPrefab(defaultPlayerPrefab);

	Scene* sceneStartup = sceneManager.createScene("startupScene");

	Camera* camera = new Camera;
	camera->setTag("MainCamera");
	camera->setActive(true);

	camera->setTransform(Transform(Vector2(50, 50)));
	camera->setWidth(100);
	camera->setHeight(100);

	sceneStartup->addGameObject(camera);

	GameObject* gameObjectStartup = new GameObject;
	SpriteDef logo = {"Engine_logo/Engine-BRAVO_logo.png", Rect(), 100, 100};
	Sprite* sprite = EngineBravo::getInstance().getResourceManager().createSprite(logo);

	gameObjectStartup->addComponent(sprite);

	sceneStartup->addGameObject(gameObjectStartup);

	GameObject* startupBehaviourObject = new GameObject;
	startupBehaviourObject->addComponent<StartupBehaviourScript>();
	sceneStartup->addGameObject(startupBehaviourObject);

	sceneManager.requestSceneChange("startupScene");

	// Scene* scene = sceneManager.createScene("tempScene");
	// GameObject* tempObject = new GameObject;

	// tempObject->addComponent<InitBehaviourScript>();

	// scene->addGameObject(tempObject);

	// int cameraID = scene->addCamera();
	// scene->setActiveCamera(cameraID);

	// scene->getActiveCamera().setTransform(Transform(Vector2(80, 96)));
	// scene->getActiveCamera().setWidth(16 * 30);
	// scene->getActiveCamera().setHeight(9 * 30);

	// sceneManager.requestSceneChange("tempScene");

	engine.run();

	return;
}

int main()
{
	engineTest();

	return 0;
}
