#include "StartupBehaviourScript.h"

#include "Engine/EngineBravo.h"
#include "NetworkBehaviourScript.h"

void StartupBehaviourScript::onStart() { mStartTime = std::chrono::system_clock::now(); }

void StartupBehaviourScript::onUpdate()
{
	auto currentTime = std::chrono::system_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - mStartTime).count();

	if (duration > 300)
	{
		EngineBravo& engine = EngineBravo::getInstance();
		SceneManager& sceneManager = engine.getSceneManager();
		Scene* scene = sceneManager.createScene("networkScene");

		int cameraIDNetwork = scene->addCamera();
		scene->setActiveCamera(cameraIDNetwork);

		scene->getActiveCamera().setTransform(Transform(Vector2(100, 100)));
		scene->getActiveCamera().setWidth(200);
		scene->getActiveCamera().setHeight(200);

		GameObject* gameObject = new GameObject;

		gameObject->addComponent<NetworkBehaviourScript>();

		scene->addGameObject(gameObject);

		sceneManager.requestSceneChange("networkScene");
	}
}
