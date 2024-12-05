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

	void setAspectRatio(const Point& aAspectRatio);
	Point getAspectRatio() const;

	void renderLayer(const Scene& aScene, int aLayer, const Camera& aCurrentCamera, const Rect& aScreenViewPort) const;
	void render(const Scene& aScene) const;

	Renderer& getRenderer();
	Window& getWindow();

	bool getTextSize(const std::string& aFont, const std::string& aText, int& aWidth, int& aHeight,
					 const Vector2& aScale) const;
	Vector2 screenToWorldPos(const Point& aScreenpos, const Camera& aCurrentCamera) const;

public:
	void addObject(GameObject& aObject);
	void removeObject(GameObject& aObject);

	const std::vector<std::reference_wrapper<GameObject>>& getObjects() const;
	void clearObjects();

private:
	void renderSprite(const Camera& aCurrentCamera, const GameObject& aGameObject, const Sprite& aSprite,
					  const Rect& aScreenViewPort) const;
	void renderAnimation(const Camera& aCurrentCamera, const GameObject& aGameObject, const Animation& aAnimation,
						 const Rect& aScreenViewPort) const;
	void renderParticle(const Camera& aCurrentCamera, const Particle& aParticle, const Rect& aScreenViewPort) const;
	void renderText(const Camera& aCurrentCamera, const std::string& aText, const Vector2& aLocation,
					const Color& aColor, const Vector2& aScale, const Rect& aScreenViewPort) const;
	void renderSquare(const Vector2& aPosition, const int aWidth, const int aHeight, const float aRotation,
					  const Color& aColor, bool aFilled, const Camera& aCurrentCamera,
					  const Rect& aScreenViewPort) const;
	void renderCircle(const Vector2& aPosition, const float aRadius, const Color& aColor, const bool aFilled,
					  const Camera& aCurrentCamera, const Rect& aScreenViewPort) const;

	void renderForCamera(const Scene& aScene, const Camera& camera, const Rect& aScreenViewPort) const;

	void renderDebugInfo(const Scene& aScene, const Camera& aCurrentCamera, const Rect& aScreenViewPort) const;

	void sortCamerasByRenderOrder(std::vector<Camera*>& aCameras) const;

	int getLowestLayer(const Scene& aScene) const;
	int getHighestLayer(const Scene& aScene) const;

private:
	std::vector<std::reference_wrapper<GameObject>> mObjects;
	std::unique_ptr<Renderer> mRenderer;
	std::unique_ptr<Window> mWindow;

	Point mAspectRatio;

	int WindowWidth;
	int WindowHeight;

	Color mBackgroundColor;
};

#endif
