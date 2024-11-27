#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <memory>

#include "Button.h"
#include "Particle.h"
#include "Point.h"
#include "Renderer.h"
#include "Scene.h"
#include "Sprite.h"
#include "Window.h"

class RenderSystem
{
public:
	RenderSystem();

<<<<<<< HEAD
	void renderLayer(Scene* aScene, int aLayer, Camera& aCurrentCamera, Rect aScreenViewPort);
=======
	void renderLayer(Scene* aScene, int aLayer);
>>>>>>> development
	void render(Scene* aScene);

	Renderer& getRenderer();
	Window& getWindow();

	bool getTextSize(const std::string& aFont, const std::string& aText, int& aWidth, int& aHeight, Vector2 aScale);
<<<<<<< HEAD
	Vector2 screenToWorldPos(Point aScreenpos, Camera& aCurrentCamera);
=======
>>>>>>> development

public:
	void addObject(GameObject& aObject);
	void removeObject(GameObject& aObject);
<<<<<<< HEAD

private:
	void renderSprite(Camera& aCurrentCamera, GameObject* aGameObject, Sprite* aSprite, Rect aScreenViewPort);
	void renderAnimation(Camera& aCurrentCamera, GameObject* aGameObject, Animation* aAnimation, Rect aScreenViewPort);
	void renderParticle(Camera& aCurrentCamera, Particle& aParticle, Rect aScreenViewPort);
	void renderText(Camera& aCurrentCamera, const std::string& aText, Vector2 aLocation, Color aColor, Vector2 aScale,
					Rect aScreenViewPort);

	void renderForCamera(Scene* aScene, Camera& camera, Rect aScreenViewPort);

	void renderDebugInfo(Scene* aScene, Camera& aCurrentCamera);
=======
	const std::vector<std::reference_wrapper<GameObject>>& getObjects() const;

private:
	void renderSprite(Camera& aCurrentCamera, GameObject* aGameObject, Sprite* aSprite);
	void renderAnimation(Camera& aCurrentCamera, GameObject* aGameObject, Animation* aAnimation);
	void renderParticle(Camera& aCurrentCamera, Particle& aParticle);
	void renderText(Camera& aCurrentCamera, const std::string& aText, Vector2 aLocation, Color aColor, Vector2 aScale);

	// void renderButton(Camera& aCurrentCamera, Button* aButton);

	void renderDebugInfo(Scene* aScene);
>>>>>>> development

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
