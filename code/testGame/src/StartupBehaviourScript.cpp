#include "StartupBehaviourScript.h"

#include "Engine/EngineBravo.h"
#include "NetworkBehaviourScript.h"

void StartupBehaviourScript::onStart() { mStartTime = std::chrono::system_clock::now(); }

void StartupBehaviourScript::onUpdate() {
    auto currentTime = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - mStartTime).count();

    if (duration > 2000) {
        EngineBravo& engine = EngineBravo::getInstance();
        SceneManager& sceneManager = engine.getSceneManager();
        Scene* scene = sceneManager.createScene("networkScene");

        Camera* camera = new Camera;
        camera->setTag("MainCamera");
        camera->setActive(true);

        camera->setTransform(Transform(Vector2(100, 100)));
        camera->setWidth(200);
        camera->setHeight(200);

        scene->addGameObject(camera);

        GameObject* gameObject = new GameObject;

        gameObject->addComponent<NetworkBehaviourScript>();

        scene->addGameObject(gameObject);

        sceneManager.requestSceneChange("networkScene");
    }
}
