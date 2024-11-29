#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <memory>

#include "Button.h"
#include "Particle.h"
#include "Renderer.h"
#include "Scene.h"
#include "Sprite.h"
#include "Window.h"

class RenderSystem
{
public:
	RenderSystem();

	void renderLayer(Scene* aScene, int aLayer);
	void render(Scene* aScene);

	Renderer& getRenderer();
	Window& getWindow();

	bool getTextSize(const std::string& aFont, const std::string& aText, int& aWidth, int& aHeight, Vector2 aScale);

public:
	void addObject(GameObject& aObject);
	void removeObject(GameObject& aObject);
	const std::vector<std::reference_wrapper<GameObject>>& getObjects() const;

private:
	void renderSprite(Camera& aCurrentCamera, GameObject* aGameObject, Sprite* aSprite);
	void renderAnimation(Camera& aCurrentCamera, GameObject* aGameObject, Animation* aAnimation);
	void renderParticle(Camera& aCurrentCamera, Particle& aParticle);
	void renderText(Camera& aCurrentCamera, const std::string& aText, Vector2 aLocation, Color aColor, Vector2 aScale);

	// void renderButton(Camera& aCurrentCamera, Button* aButton);

	void renderDebugInfo(Scene* aScene);

	int getLowestLayer(Scene* aScene);
	int getHighestLayer(Scene* aScene);

private:
	std::vector<std::reference_wrapper<GameObject>> mObjects;
	std::unique_ptr<Renderer> mRenderer;
	std::unique_ptr<Window> mWindow;

	int WindowWidth;
	int WindowHeight;

	Color mBackgroundColor;
};

#endif
