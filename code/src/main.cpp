#include <string>
#include <vector>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_render.h>
#include <SDL_video.h>

#include "Animation.h"
#include "EngineBravo.h"
#include "FSConverter.h"
#include "PlayerBehaviourScript.h"
#include "Renderer.h"
#include "SampleBevahiourScript.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Sprite.h"
#include "SpriteAtlas.h"
#include "SpriteDef.h"
#include "Texture.h"
#include "Window.h"
#include "test.h"

void engineTest() {
    EngineBravo& engine = EngineBravo::getInstance();
    SceneManager& sceneManager = engine.getSceneManager();

    Scene* scene = sceneManager.createScene("Level1");
    if (scene == nullptr)
        exit(1);

    GameObject* gameObject = new GameObject;

    Transform objectTransform;

    objectTransform.position.x = 400;
    objectTransform.position.y = 400;

    gameObject->setTransform(objectTransform);

    gameObject->addComponent<PlayerBehaviourScript>();

    scene->addGameObject(gameObject);

    sceneManager.loadScene(0);

    engine.initizalize();
    engine.run();

    return;
}

int main() { engineTest(); }
