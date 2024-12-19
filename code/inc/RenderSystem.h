/**
 * @file RenderSystem.h
 * @brief Defines the RenderSystem class, responsible for managing and rendering game objects, scenes, and graphical
 * elements.
 */

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

/**
 * @class RenderSystem
 * @brief Handles rendering of game objects, scenes, and various graphical elements.
 *
 * This class manages a collection of game objects, interacts with the Renderer and Window classes,
 * and provides functionality to render scenes and graphical components efficiently.
 */
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
					  const Color& aColor, bool aFilled, const Camera& aCurrentCamera, const Rect& aScreenViewPort,
					  const Vector2& aScale = {1, 1}, const Point& aRotationalCenter = {0, 0}) const;
	void renderCircle(const Vector2& aPosition, const float aRadius, const Color& aColor, const bool aFilled,
					  const Camera& aCurrentCamera, const Rect& aScreenViewPort) const;

	void renderForCamera(const Scene& aScene, const Camera& camera, const Rect& aScreenViewPort) const;

	void renderDebugInfo(const Scene& aScene, const Camera& aCurrentCamera, const Rect& aScreenViewPort) const;

	void sortCamerasByRenderOrder(std::vector<Camera*>& aCameras) const;

	// int getLowestLayer(const Scene& aScene) const;
	// int getHighestLayer(const Scene& aScene) const;
	void updateLayerRange(GameObject& aObject);

private:
	std::vector<std::reference_wrapper<GameObject>> mObjects; ///< Collection of game objects managed by the system.
	std::unique_ptr<Renderer> mRenderer;					  ///< Renderer instance for handling graphical output.
	std::unique_ptr<Window> mWindow;						  ///< Window instance for displaying rendered content.

	Point mAspectRatio; ///< Aspect ratio for rendering.

	int WindowWidth;  ///< Width of the rendering window.
	int WindowHeight; ///< Height of the rendering window.

	int mLowestLayer;  ///< Lowest layer in the scene.
	int mHighestLayer; ///< Highest layer in the scene.

	Color mBackgroundColor; ///< Background color for rendering.
};

#endif
