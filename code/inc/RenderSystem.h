#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <memory>

#include "Renderer.h"
#include "Scene.h"
#include "Sprite.h"
#include "Window.h"

class RenderSystem {
public:
    RenderSystem();

    void render(Scene* aScene);

    Renderer& getRenderer();
    Window& getWindow();

private:
    void renderSprite(GameObject* aGameObject, Sprite* aSprite);
    void renderAnimation(GameObject* aGameObject, Animation* aAnimation);

    std::unique_ptr<Renderer> mRenderer;
    std::unique_ptr<Window> mWindow;

    int WindowWidth;
    int WindowHeight;

    Color mBackgroundColor;
};

#endif
