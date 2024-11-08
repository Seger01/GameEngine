#include <chrono>
#include <string>
#include <thread>
#include <vector>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_render.h>
#include <SDL_video.h>

#include "Animation.h"
#include "EngineBravo.h"
#include "FSConverter.h"
#include "GameObject.h"
#include "InitBehaviourScript.h"
#include "PlayerBehaviourScript.h"
#include "Renderer.h"
#include "SampleBevahiourScript.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Sprite.h"
#include "SpriteAtlas.h"
#include "SpriteDef.h"
#include "Texture.h"
#include "TileMapParser.h"
#include "Transform.h"
#include "Window.h"
#include "test.h"

void engineTest() {
    EngineBravo& engine = EngineBravo::getInstance();
    SceneManager& sceneManager = engine.getSceneManager();

    Scene* scene = sceneManager.createScene("initscene");
    if (scene == nullptr)
        exit(1);

    int cameraID = scene->addCamera();
    scene->setActiveGamera(cameraID);

    scene->getActiveCamera().setTransform(Transform(Vector2(80, 0)));
    scene->getActiveCamera().setWidth(160);
    scene->getActiveCamera().setHeight(90);

    GameObject* gameObject = new GameObject;

    gameObject->addComponent<InitBehaviourScript>();

    scene->addGameObject(gameObject);

    sceneManager.requestSceneChange("initscene");

    engine.initizalize();
    engine.run();

    return;
}

void physicsTest() {
    // Create a World instance with gravity
    World physicsWorld;
    Vector2 gravity(0.0f, -9.8f); // Set gravity to mimic Earth's gravity
    physicsWorld.createWorld(gravity);

    // Create a test GameObject and wrap it in BodyProxy
    GameObject testObject;
    testObject.setTransform(Transform(Vector2(0, 10)));
    RigidBody* rigidBody = new RigidBody();

    rigidBody->setCanRotate(true);
    rigidBody->setHasGravity(true);
    rigidBody->setIsMoveableByForce(true);
    rigidBody->setDensity(1.0f);
    rigidBody->setFriction(0.3f);
    rigidBody->setRestitution(0.2f);
    rigidBody->setGravityScale(1.0f);
    rigidBody->setMass(1.0f);

    testObject.addComponent(rigidBody);

    BoxCollider* boxCollider = new BoxCollider();
    boxCollider->setWidth(10);
    boxCollider->setHeight(10);

    testObject.addComponent(boxCollider);

    BodyProxy bodyProxy(&testObject);

    // Add body to the world
    int bodyID = physicsWorld.createBody(bodyProxy);

    // Apply an initial force to move the body upward
    physicsWorld.applyForce(bodyID, Vector2(0.0f, 100.0f));

    // Run the simulation for a few steps
    for (int i = 0; i < 180; ++i) {                     // Simulate 1 second at 60 FPS
        physicsWorld.executeWorldStep(1.0f / 60.0f, 8); // Step with a delta time of 1/60 seconds

        // Get and print the updated position of the body
        Vector2 position = physicsWorld.getPosition(bodyID);
        std::cout << "Step " << i << ": Position = (" << position.x << ", " << position.y << ")" << std::endl;

        // Wait to simulate real-time (optional)
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    return;
}

int main() {
    // engineTest();
    physicsTest();
    return 0;
}
