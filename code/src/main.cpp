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

void physicsInEngineTest() {

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

    // Create a GameObject and add components
    GameObject* object = new GameObject();

    object->setName("Falling Box");
    object->setID(1);

    // Add a BoxCollider to the GameObject
    BoxCollider* boxCollider = new BoxCollider();
    boxCollider->setWidth(10.0f);
    boxCollider->setHeight(10.0f);
    object->addComponent(boxCollider);

    // Add a RigidBody component to the GameObject to make it a dynamic body
    RigidBody* rigidBody = new RigidBody();
    rigidBody->setHasGravity(true);
    rigidBody->setDensity(1.0f);
    rigidBody->setFriction(0.3f);
    rigidBody->setRestitution(0.2f);
    rigidBody->setMass(1.0f);
    rigidBody->setGravityScale(1.0f);
    rigidBody->setCanRotate(false);
    object->addComponent(rigidBody);

    scene->addGameObject(object);

    PhysicsEngine physicsEngine;
    physicsEngine.createWorld(Vector2(0.0f, -9.8f));

    physicsEngine.updateReferences(scene->getGameObjects());

    physicsEngine.createBodies();

    physicsEngine.setStep(1.0f / 60.0f);
    physicsEngine.setSubStep(6);

    for (int i = 0; i < 60; ++i) { // Run simulation for 1 second
        physicsEngine.update();

        std::cout << "Object location: " << object->getTransform().position.x << ", "
                  << object->getTransform().position.y << std::endl;

        if (i == 60) {
            // Apply a force to the object after 1 second
        }
    }
}

void physicsTest() {

    // Initialize World with gravity
    World physicsWorld;
    Vector2 gravity = {0.0f, -9.8f};
    physicsWorld.createWorld(gravity);

    // Create a GameObject and add components
    GameObject object;
    object.setName("Falling Box");
    object.setID(1);

    // Add a BoxCollider to the GameObject
    BoxCollider* boxCollider = new BoxCollider();
    boxCollider->setWidth(1.0f);
    boxCollider->setHeight(1.0f);
    object.addComponent(boxCollider);

    // Add a RigidBody component to the GameObject to make it a dynamic body
    RigidBody* rigidBody = new RigidBody();
    rigidBody->setHasGravity(true);
    rigidBody->setDensity(1.0f);
    rigidBody->setFriction(0.3f);
    rigidBody->setRestitution(0.2f);
    rigidBody->setMass(1.0f);
    rigidBody->setGravityScale(1.0f);
    rigidBody->setCanRotate(false);
    object.addComponent(rigidBody);

    // Create a BodyProxy from the GameObject
    BodyProxy bodyProxy(&object);

    // Add the body to the physics world
    int bodyID = physicsWorld.createBody(bodyProxy);

    // Simulate the world for a few steps
    float timeStep = 1.0f / 60.0f;
    int velocityIterations = 6;
    int positionIterations = 2;

    for (int i = 0; i < 240; ++i) { // Run simulation for 1 second
        physicsWorld.executeWorldStep(timeStep, velocityIterations);

        if (i == 60) {
            // Apply a force to the object after 1 second
            physicsWorld.applyForce(bodyID, Vector2(10000.0f, 0.0f));
        }

        if (i == 120) {
            BodyProxy bodyProxy2(&object);
            physicsWorld.updateBody(bodyID, bodyProxy2);
        }

        if (i == 180) {
            object.setTransform(Transform(Vector2(69.0f, 420.0f)));

            BodyProxy bodyProxy3(&object);

            physicsWorld.updateBody(bodyID, bodyProxy3);
        }

        // Get the updated position of the object
        Vector2 position = physicsWorld.getPosition(bodyID);

        // Output the object's position
        std::cout << "Step " << i << ": Object Position = (" << position.x << ", " << position.y << ")\n";
    }
}

void physicsTest2() {
    // Initialize World with gravity
    // World physicsWorld;
    Vector2 gravity = {0.0f, -9.8f};
    // physicsWorld.createWorld(gravity);
    PhysicsEngine physicsEngine;
    physicsEngine.createWorld(gravity);

    World& physicsWorld = physicsEngine.getWorld();

    // Create a GameObject and add components
    GameObject object;
    object.setName("Falling Box");
    object.setID(1);

    // Add a BoxCollider to the GameObject
    BoxCollider* boxCollider = new BoxCollider();
    boxCollider->setWidth(1.0f);
    boxCollider->setHeight(1.0f);
    object.addComponent(boxCollider);

    // Add a RigidBody component to the GameObject to make it a dynamic body
    RigidBody* rigidBody = new RigidBody();
    rigidBody->setHasGravity(true);
    rigidBody->setDensity(1.0f);
    rigidBody->setFriction(0.3f);
    rigidBody->setRestitution(0.2f);
    rigidBody->setMass(1.0f);
    rigidBody->setGravityScale(1.0f);
    rigidBody->setCanRotate(false);
    object.addComponent(rigidBody);

    // // Create a BodyProxy from the GameObject
    // BodyProxy bodyProxy(&object);
    //
    // // Add the body to the physics world
    // int bodyID = physicsWorld.createBody(bodyProxy);
    int bodyID = 1;
    physicsEngine.setgameObjects(std::vector<GameObject*>{&object});
    physicsEngine.createBodies();

    // Simulate the world for a few steps
    float timeStep = 1.0f / 60.0f;
    int velocityIterations = 6;
    int positionIterations = 2;

    for (int i = 0; i < 240; ++i) { // Run simulation for 1 second
        // physicsWorld.executeWorldStep(timeStep, velocityIterations);
        physicsEngine.update();

        if (i == 120) {
            // BodyProxy bodyProxy2(&object);
            // physicsWorld.updateBody(bodyID, bodyProxy2);
            object.setTransform(Transform(Vector2(69.0f, 420.0f)));
        }

        // Get the updated position of the object
        // Vector2 position = physicsWorld.getPosition(bodyID);
        Vector2 position = object.getTransform().position;

        // Output the object's position
        std::cout << "Step " << i << ": Object Position = (" << position.x << ", " << position.y << ")\n";
    }
}

void physicsEngineTest() {
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

    // Create a GameObject and add components
    GameObject* object = new GameObject();

    object->setName("Falling Box");
    object->setID(1);

    // Add a BoxCollider to the GameObject
    BoxCollider* boxCollider = new BoxCollider();
    boxCollider->setWidth(10.0f);
    boxCollider->setHeight(10.0f);
    object->addComponent(boxCollider);

    // Add a RigidBody component to the GameObject to make it a dynamic body
    RigidBody* rigidBody = new RigidBody();
    rigidBody->setHasGravity(true);
    rigidBody->setDensity(10.0f);
    rigidBody->setFriction(0.0f);
    rigidBody->setRestitution(0.2f);
    rigidBody->setMass(10.0f);
    rigidBody->setGravityScale(10.0f);
    rigidBody->setCanRotate(false);
    object->addComponent(rigidBody);

    scene->addGameObject(object);

    sceneManager.requestSceneChange("initscene");

    engine.initizalize();
    engine.run();
}

int main() {
    engineTest();
    // physicsTest();
    // physicsTest2();
    // physicsInEngineTest();
    // physicsEngineTest();
    return 0;
}
