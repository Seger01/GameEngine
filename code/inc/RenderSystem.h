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

	void renderLayer(Scene* aScene, int aLayer, Camera& aCurrentCamera, Rect aScreenViewPort);
	void render(Scene* aScene);

	Renderer& getRenderer();
	Window& getWindow();

	bool getTextSize(const std::string& aFont, const std::string& aText, int& aWidth, int& aHeight, Vector2 aScale);
	Vector2 screenToWorldPos(Point aScreenpos, Camera& aCurrentCamera);

public:
	void addObject(GameObject& aObject);
	void removeObject(GameObject& aObject);

	const std::vector<std::reference_wrapper<GameObject>>& getObjects() const;
	void clearObjects();

private:
	void renderSprite(Camera& aCurrentCamera, GameObject* aGameObject, Sprite* aSprite, Rect aScreenViewPort);
	void renderAnimation(Camera& aCurrentCamera, GameObject* aGameObject, Animation* aAnimation, Rect aScreenViewPort);
	void renderParticle(Camera& aCurrentCamera, Particle& aParticle, Rect aScreenViewPort);
	void renderText(Camera& aCurrentCamera, const std::string& aText, Vector2 aLocation, Color aColor, Vector2 aScale,
					Rect aScreenViewPort);
	void renderSquare(Vector2 aPosition, int aWidth, int aHeight, float aRotation, Color aColor, bool aFilled,
					  Camera& aCurrentCamera, Rect aScreenViewPort);
	void renderCircle(Vector2 aPosition, float aRadius, Color aColor, bool aFilled, Camera& aCurrentCamera,
					  Rect aScreenViewPort);

	void renderForCamera(Scene* aScene, Camera& camera, Rect aScreenViewPort);

	void renderDebugInfo(Scene* aScene, Camera& aCurrentCamera, Rect aScreenViewPort);

	void sortCamerasByRenderOrder(std::vector<Camera*>& aCameras);

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
