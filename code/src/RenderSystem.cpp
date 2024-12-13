/**
 * @file RenderSystem.cpp
 * @brief Implementation of the RenderSystem class responsible for rendering various game objects and elements.
 */

#include "RenderSystem.h"

#include <cmath>
#include <memory>

#include "Animation.h"
#include "BoxCollider.h"
#include "CircleCollider.h"
#include "Color.h"
#include "ParticleEmitter.h"
#include "Sprite.h"
#include "Text.h"
#include "Time.h"

/**
 * @brief Constructor for the RenderSystem class. Initializes the window, renderer, and default settings.
 */
RenderSystem::RenderSystem() : WindowWidth(800), WindowHeight(450), mAspectRatio(Point{16, 9})
{
	mWindow = std::make_unique<Window>(WindowWidth, WindowHeight);
	mRenderer = std::make_unique<Renderer>(*mWindow);

	mBackgroundColor = Color(255, 255, 255);

	return;
}

/**
 * @brief Sets the aspect ratio for the rendering system.
 * @param aAspectRatio The new aspect ratio as a Point.
 */
void RenderSystem::setAspectRatio(const Point& aAspectRatio) { mAspectRatio = aAspectRatio; }

/**
 * @brief Retrieves the current aspect ratio of the rendering system.
 * @return The aspect ratio as a Point.
 */
Point RenderSystem::getAspectRatio() const { return mAspectRatio; }

/**
 * @brief Renders a Sprite component of a GameObject.
 * @param aCurrentCamera The active camera rendering the scene.
 * @param aGameObject The GameObject to which the Sprite belongs.
 * @param aSprite The Sprite component to render.
 * @param aScreenViewPort The viewport of the screen.
 */
void RenderSystem::renderSprite(const Camera& aCurrentCamera, const GameObject& aGameObject, const Sprite& aSprite,
								const Rect& aScreenViewPort) const
{
	Vector2 texturePosition = aGameObject.getTransform().position + aSprite.getRelativePosition().position;
	Vector2 cameraOrigin = aCurrentCamera.getOrigin();
	Vector2 drawPosition = texturePosition - cameraOrigin;

	Rect cameraRect = {static_cast<int>(cameraOrigin.x), static_cast<int>(cameraOrigin.y),
					   static_cast<int>(aCurrentCamera.getWidth()), static_cast<int>(aCurrentCamera.getHeight())};

	Rect spriteRect = {static_cast<int>(texturePosition.x), static_cast<int>(texturePosition.y), aSprite.getWidth(),
					   aSprite.getHeight()};

	if (!cameraRect.intersects(spriteRect))
	{
		return;
	}

	// Adjust draw position and size to the viewport
	drawPosition.x = std::round(drawPosition.x * ((float)aScreenViewPort.w / aCurrentCamera.getWidth()));
	drawPosition.y = std::round(drawPosition.y * ((float)aScreenViewPort.h / aCurrentCamera.getHeight()));

	int spriteWidth = std::round(aSprite.getWidth() * ((float)aScreenViewPort.w / aCurrentCamera.getWidth())) + 1;
	int spriteHeight = std::round(aSprite.getHeight() * ((float)aScreenViewPort.h / aCurrentCamera.getHeight())) + 1;

	// Render
	mRenderer->renderTexture(aSprite.getTexture(), aSprite.getSource(), drawPosition, spriteWidth, spriteHeight,
							 aSprite.getFlipX(), aSprite.getFlipY(),
							 aGameObject.getTransform().rotation + aSprite.getRelativePosition().rotation,
							 aSprite.getColorFilter());
}

/**
 * @brief Renders an Animation component of a GameObject.
 * @param aCurrentCamera The active camera rendering the scene.
 * @param aGameObject The GameObject to which the Animation belongs.
 * @param aAnimation The Animation component to render.
 * @param aScreenViewPort The viewport of the screen.
 */
void RenderSystem::renderAnimation(const Camera& aCurrentCamera, const GameObject& aGameObject,
								   const Animation& aAnimation, const Rect& aScreenViewPort) const
{
	const Sprite& currentFrame = aAnimation.getCurrentFrame();

	renderSprite(aCurrentCamera, aGameObject, currentFrame, aScreenViewPort);
}

/**
 * @brief Renders a Particle instance.
 * @param aCurrentCamera The active camera rendering the scene.
 * @param aParticle The Particle to render.
 * @param aScreenViewPort The viewport of the screen.
 */
void RenderSystem::renderParticle(const Camera& aCurrentCamera, const Particle& aParticle,
								  const Rect& aScreenViewPort) const
{
	float particleWidth = aParticle.getSize().x;
	float particleHeight = aParticle.getSize().y;

	Vector2 particlePosition = aParticle.getPosition();
	Vector2 cameraOrigin = aCurrentCamera.getOrigin();
	Vector2 drawPosition = particlePosition - cameraOrigin;

	drawPosition.x = drawPosition.x * (static_cast<float>(aScreenViewPort.w) / aCurrentCamera.getWidth());
	drawPosition.y = drawPosition.y * (static_cast<float>(aScreenViewPort.h) / aCurrentCamera.getHeight());

	particleWidth =
		static_cast<int>(static_cast<float>(particleWidth) *
						 (static_cast<float>(aScreenViewPort.w) / static_cast<float>(aCurrentCamera.getWidth())));
	particleHeight =
		static_cast<int>(static_cast<float>(particleHeight) *
						 (static_cast<float>(aScreenViewPort.h) / static_cast<float>(aCurrentCamera.getHeight())));

	mRenderer->renderSquare(drawPosition, static_cast<int>(particleWidth), static_cast<int>(particleHeight),
							aParticle.getRotation(), aParticle.getColor(), true);
}

/**
 * @brief Renders text on the screen.
 * @param aCurrentCamera The active camera rendering the scene.
 * @param aText The text to render.
 * @param aLocation The position of the text.
 * @param aColor The color of the text.
 * @param aScale The scale of the text.
 * @param aScreenViewPort The viewport of the screen.
 */
void RenderSystem::renderText(const Camera& aCurrentCamera, const std::string& aText, const Vector2& aLocation,
							  const Color& aColor, const Vector2& aScale, const Rect& aScreenViewPort) const
{
	float scaleX = aScale.x * (aScreenViewPort.w / static_cast<float>(aCurrentCamera.getWidth()));
	float scaleY = aScale.y * (aScreenViewPort.h / static_cast<float>(aCurrentCamera.getHeight()));

	Vector2 cameraOrigin = aCurrentCamera.getOrigin();
	Vector2 drawPosition = aLocation - cameraOrigin;

	drawPosition.x = drawPosition.x * (static_cast<float>(aScreenViewPort.w) / aCurrentCamera.getWidth());
	drawPosition.y = drawPosition.y * (static_cast<float>(aScreenViewPort.h) / aCurrentCamera.getHeight());

	mRenderer->renderText(aText, drawPosition, aColor, scaleX, scaleY);
}

/**
 * @brief Retrieves the size of a text string.
 * @param aFont The font to use.
 * @param aText The text to measure.
 * @param aWidth The width of the text.
 * @param aHeight The height of the text.
 * @param aScale The scale of the text.
 * @return True if the text size was successfully retrieved, false otherwise.
 */
bool RenderSystem::getTextSize(const std::string& aFont, const std::string& aText, int& aWidth, int& aHeight,
							   const Vector2& aScale) const
{
	if (!mRenderer->calculateTextSize(aFont, aText, aWidth, aHeight))
	{
		return false;
	}

	aWidth = aWidth * aScale.x;
	aHeight = aHeight * aScale.y;

	return true;
}

/**
 * @brief Converts a screen position to a world position.
 * @param aScreenpos The screen position to convert.
 * @param aCurrentCamera The active camera rendering the scene.
 * @return The world position as a Vector2.
 */
Vector2 RenderSystem::screenToWorldPos(const Point& aScreenpos, const Camera& aCurrentCamera) const
{
	Vector2 screenPos{static_cast<float>(aScreenpos.x), static_cast<float>(aScreenpos.y)};
	FRect viewport = aCurrentCamera.getViewport();

	// Determine the actual screen viewport after letterboxing/pillarboxing
	Rect screenViewPort =
		Rect{static_cast<int>(viewport.x * mWindow->getSize().x), static_cast<int>(viewport.y * mWindow->getSize().y),
			 static_cast<int>(viewport.w * mWindow->getSize().x), static_cast<int>(viewport.h * mWindow->getSize().y)};

	if ((mWindow->getSize().x / mAspectRatio.x) < (mWindow->getSize().y / mAspectRatio.y))
	{
		// Letterbox
		int newHeight = mWindow->getSize().x / mAspectRatio.x * mAspectRatio.y;
		screenViewPort.y = ((mWindow->getSize().y - newHeight) / 2) + (newHeight * viewport.y);
		screenViewPort.h = newHeight * viewport.h;
	}
	else if ((mWindow->getSize().x / mAspectRatio.x) > (mWindow->getSize().y / mAspectRatio.y))
	{
		// Pillarbox
		int newWidth = mWindow->getSize().y / mAspectRatio.y * mAspectRatio.x;
		screenViewPort.x = ((mWindow->getSize().x - newWidth) / 2) + (newWidth * viewport.x);
		screenViewPort.w = newWidth * viewport.w;
	}

	// Adjust screen position to viewport coordinates
	screenPos.x = screenPos.x - screenViewPort.x;
	screenPos.y = screenPos.y - screenViewPort.y;

	// Convert screen position to world position
	Vector2 worldPos;
	worldPos.x = (screenPos.x / screenViewPort.w) * aCurrentCamera.getWidth();
	worldPos.y = (screenPos.y / screenViewPort.h) * aCurrentCamera.getHeight();

	// Offset by the camera's origin
	worldPos.x += aCurrentCamera.getOrigin().x;
	worldPos.y += aCurrentCamera.getOrigin().y;

	return worldPos;
}

/**
 * @brief Function that finds the lowest layer in the scene.
 * @param aScene The scene to search.
 * @return The lowest layer as an integer.
 */
int RenderSystem::getLowestLayer(const Scene& aScene) const
{
	int lowestLayer = 0;
	for (auto& gameObject : aScene.getGameObjects())
	{
		if (gameObject->hasComponent<Sprite>())
		{
			for (auto sprite : gameObject->getComponents<Sprite>())
			{
				if (sprite->getLayer() < lowestLayer)
				{
					lowestLayer = sprite->getLayer();
				}
			}
		}
		if (gameObject->hasComponent<Animation>())
		{
			for (auto animation : gameObject->getComponents<Animation>())
			{
				if (animation->getLayer() < lowestLayer)
				{
					lowestLayer = animation->getLayer();
				}
			}
		}
		if (gameObject->hasComponent<ParticleEmitter>())
		{
			for (auto particleEmitter : gameObject->getComponents<ParticleEmitter>())
			{
				if (particleEmitter->getLayer() < lowestLayer)
				{
					lowestLayer = particleEmitter->getLayer();
				}
			}
		}
		if (typeid(*gameObject) == typeid(Text))
		{
			Text& text = dynamic_cast<Text&>(*gameObject);
			if (text.getLayer() < lowestLayer)
			{
				lowestLayer = text.getLayer();
			}
		}
	}
	return lowestLayer;
}

/**
 * @brief Function that finds the highest layer in the scene.
 * @param aScene The scene to search.
 * @return The highest layer as an integer.
 */
int RenderSystem::getHighestLayer(const Scene& aScene) const
{
	int highestLayer = 0;
	for (auto& gameObject : aScene.getGameObjects())
	{
		if (gameObject->hasComponent<Sprite>())
		{
			for (auto sprite : gameObject->getComponents<Sprite>())
			{
				if (sprite->getLayer() > highestLayer)
				{
					highestLayer = sprite->getLayer();
				}
			}
		}
		if (gameObject->hasComponent<Animation>())
		{
			for (auto animation : gameObject->getComponents<Animation>())
			{
				if (animation->getLayer() > highestLayer)
				{
					highestLayer = animation->getLayer();
				}
			}
		}
		if (gameObject->hasComponent<ParticleEmitter>())
		{
			for (auto particleEmitter : gameObject->getComponents<ParticleEmitter>())
			{
				if (particleEmitter->getLayer() > highestLayer)
				{
					highestLayer = particleEmitter->getLayer();
				}
			}
		}
		if (typeid(*gameObject) == typeid(Text))
		{
			Text& text = dynamic_cast<Text&>(*gameObject);
			if (text.getLayer() > highestLayer)
			{
				highestLayer = text.getLayer();
			}
		}
	}
	return highestLayer;
}

/**
 * @brief Renders all objects in a specific layer.
 * @param aScene The scene to render.
 * @param aLayer The layer to render.
 * @param activeCamera The active camera rendering the scene.
 * @param aScreenViewPort The viewport of the screen.
 */
void RenderSystem::renderLayer(const Scene& aScene, int aLayer, const Camera& activeCamera,
							   const Rect& aScreenViewPort) const
{
	for (GameObject& gameObject : mObjects)
	{
		if (!gameObject.isActive())
		{
			continue;
		}
		for (auto animation : gameObject.getComponents<Animation>())
		{
			if (animation->isActive() && animation->getLayer() == aLayer)
			{
				renderAnimation(activeCamera, gameObject, *animation, aScreenViewPort);
			}
		}
		for (auto sprite : gameObject.getComponents<Sprite>())
		{
			if (sprite->isActive() && sprite->getLayer() == aLayer)
			{
				renderSprite(activeCamera, gameObject, *sprite, aScreenViewPort);
			}
		}
		for (auto particleEmitter : gameObject.getComponents<ParticleEmitter>())
		{
			if (particleEmitter->isActive() && particleEmitter->getLayer() == aLayer)
			{
				for (auto& particle : particleEmitter->getParticles())
				{
					renderParticle(activeCamera, particle, aScreenViewPort);
				}
			}
		}
		if (typeid(gameObject) == typeid(Text))
		{
			Text& text = dynamic_cast<Text&>(gameObject);
			if (text.isActive() && text.getLayer() == aLayer)
			{
				renderText(activeCamera, text.getText(), text.getTransform().position, text.getColor(), text.getScale(),
						   aScreenViewPort);
			}
		}
	}
}

/**
 * @brief Sorts the cameras by render order.
 * @param aCameras The cameras to sort.
 */
void RenderSystem::sortCamerasByRenderOrder(std::vector<Camera*>& aCameras) const
{
	std::sort(aCameras.begin(), aCameras.end(),
			  [](Camera* a, Camera* b) { return a->getRenderOrder() < b->getRenderOrder(); });
}

/**
 * @brief Renders the scene.
 * @param aScene The scene to render.
 */
void RenderSystem::render(const Scene& aScene) const
{
	mRenderer->clear(mBackgroundColor);

	// Get all active cameras
	std::vector<Camera*> cameras = aScene.getCameras();

	// Sort cameras by render order
	sortCamerasByRenderOrder(cameras);

	for (Camera* camera : cameras)
	{
		camera->update();

		FRect cameraViewport = camera->getViewport();

		Rect screenViewPort = Rect{static_cast<int>(cameraViewport.x * mWindow->getSize().x),
								   static_cast<int>(cameraViewport.y * mWindow->getSize().y),
								   static_cast<int>(cameraViewport.w * mWindow->getSize().x),
								   static_cast<int>(cameraViewport.h * mWindow->getSize().y)};

		if ((mWindow->getSize().x / mAspectRatio.x) < (mWindow->getSize().y / mAspectRatio.y))
		{
			// Letterbox
			int newHeight = mWindow->getSize().x / mAspectRatio.x * mAspectRatio.y;
			screenViewPort.y = ((mWindow->getSize().y - newHeight) / 2) + (newHeight * cameraViewport.y);
			screenViewPort.h = newHeight * cameraViewport.h;
		}
		else if ((mWindow->getSize().x / mAspectRatio.x) > (mWindow->getSize().y / mAspectRatio.y))
		{
			// Pillarbox
			int newWidth = mWindow->getSize().y / mAspectRatio.y * mAspectRatio.x;
			screenViewPort.x = ((mWindow->getSize().x - newWidth) / 2) + (newWidth * cameraViewport.x);
			screenViewPort.w = newWidth * cameraViewport.w;
		}

		mRenderer->setViewport(screenViewPort);

		// draw the camera background color
		mRenderer->renderSquare(Vector2(0, 0), screenViewPort.w, screenViewPort.h, 0, camera->getBackgroundColor(),
								true);

		// Render objects visible to this camera
		renderForCamera(aScene, *camera, screenViewPort);
	}

	mRenderer->show();
}

/**
 * @brief Renders the scene for a specific camera.
 * @param aScene The scene to render.
 * @param camera The camera to render the scene for.
 * @param aScreenViewPort The viewport of the screen.
 */
void RenderSystem::renderForCamera(const Scene& aScene, const Camera& camera, const Rect& aScreenViewPort) const
{
	int lowestLayer = getLowestLayer(aScene);
	int highestLayer = getHighestLayer(aScene);

	for (int layer = lowestLayer; layer <= highestLayer; ++layer)
	{
		renderLayer(aScene, layer, camera, aScreenViewPort);
	}

	renderDebugInfo(aScene, camera, aScreenViewPort);
}

/**
 * @brief Renders a square on the screen.
 * @param aPosition The position of the square.
 * @param aWidth The width of the square.
 * @param aHeight The height of the square.
 * @param aRotation The rotation of the square.
 * @param aColor The color of the square.
 * @param aFilled Whether the square is filled or not.
 * @param aCurrentCamera The active camera rendering the scene.
 * @param aScreenViewPort The viewport of the screen.
 */
void RenderSystem::renderSquare(const Vector2& aPosition, const int aWidth, const int aHeight, const float aRotation,
								const Color& aColor, bool aFilled, const Camera& aCurrentCamera,
								const Rect& aScreenViewPort, const Point& aRotationalCenter) const
{
	Vector2 squarePosition = aPosition;
	Vector2 cameraOrigin = aCurrentCamera.getOrigin();
	Vector2 drawPosition = squarePosition - cameraOrigin;

	// Adjust draw position and size to the viewport
	drawPosition.x = std::round(drawPosition.x * ((float)aScreenViewPort.w / aCurrentCamera.getWidth()));
	drawPosition.y = std::round(drawPosition.y * ((float)aScreenViewPort.h / aCurrentCamera.getHeight()));

	int squareWidth = std::round(aWidth * ((float)aScreenViewPort.w / aCurrentCamera.getWidth())) + 1;
	int squareHeight = std::round(aHeight * ((float)aScreenViewPort.h / aCurrentCamera.getHeight())) + 1;

	Point scaledRotationalCenter;
	scaledRotationalCenter.x = std::round(aRotationalCenter.x * ((float)aScreenViewPort.w / aCurrentCamera.getWidth()));
	scaledRotationalCenter.y =
		std::round(aRotationalCenter.y * ((float)aScreenViewPort.h / aCurrentCamera.getHeight()));

	// Render
	mRenderer->renderSquare(drawPosition, squareWidth, squareHeight, aRotation, aColor, aFilled,
							scaledRotationalCenter);
}

/**
 * @brief Renders a circle on the screen.
 * @param aPosition The position of the circle.
 * @param aRadius The radius of the circle.
 * @param aColor The color of the circle.
 * @param aFilled Whether the circle is filled or not.
 * @param aCurrentCamera The active camera rendering the scene.
 * @param aScreenViewPort The viewport of the screen.
 */
void RenderSystem::renderCircle(const Vector2& aPosition, const float aRadius, const Color& aColor, const bool aFilled,
								const Camera& aCurrentCamera, const Rect& aScreenViewPort) const
{

	Vector2 circlePosition = aPosition;
	Vector2 cameraOrigin = aCurrentCamera.getOrigin();
	Vector2 drawPosition = circlePosition - cameraOrigin;

	// Adjust draw position and size to the viewport
	drawPosition.x = std::round(drawPosition.x * ((float)aScreenViewPort.w / aCurrentCamera.getWidth()));
	drawPosition.y = std::round(drawPosition.y * ((float)aScreenViewPort.h / aCurrentCamera.getHeight()));

	int radius = std::round(aRadius * ((float)aScreenViewPort.w / aCurrentCamera.getWidth())) + 1;

	// Render
	mRenderer->drawCircle(drawPosition, radius, aColor, aFilled);
}

/**
 * @brief Renders debug information on the screen.
 * @param aScene The scene to render.
 * @param aCurrentCamera The active camera rendering the scene.
 * @param aScreenViewPort The viewport of the screen.
 */
void RenderSystem::renderDebugInfo(const Scene& aScene, const Camera& aCurrentCamera, const Rect& aScreenViewPort) const
{
	if (Time::deltaTime == 0)
	{
		return;
	}

	if (aCurrentCamera.getDebugOverlay().showFPS)
	{
		int fps = 1.0f / Time::deltaTime;

		renderText(aCurrentCamera, "FPS: " + std::to_string(fps),
				   Vector2(aCurrentCamera.getOrigin().x + 5, aCurrentCamera.getOrigin().y + 5), Color(0, 255, 0),
				   Vector2(0.5, 0.5), aScreenViewPort);
	}

	if (aCurrentCamera.getDebugOverlay().renderCameraViewport)
	{
		mRenderer->renderSquare(Vector2(0, 0), aScreenViewPort.w, aScreenViewPort.h, 0, Color(0, 255, 208), false);
	}

	if (aCurrentCamera.getDebugOverlay().renderColliders)
	{
		for (auto& gameObject : aScene.getGameObjects())
		{
			if (gameObject->hasComponent<BoxCollider>())
			{
				for (auto boxCollider : gameObject->getComponents<BoxCollider>())
				{
					Vector2 relativeBoxPosition = boxCollider->getTransform().position;

					Vector2 boxColliderWorldPos =
						gameObject->getTransform().position + boxCollider->getTransform().position;

					renderSquare(
						boxColliderWorldPos, boxCollider->getWidth(), boxCollider->getHeight(),
						gameObject->getTransform().rotation, Color(0, 0, 255), false, aCurrentCamera, aScreenViewPort,
						Point{static_cast<int>(-relativeBoxPosition.x), static_cast<int>(-relativeBoxPosition.y)});
				}
			}
			if (gameObject->hasComponent<CircleCollider>())
			{
				for (auto circleCollider : gameObject->getComponents<CircleCollider>())
				{
					Vector2 circleColliderWorldPos =
						gameObject->getTransform().position + circleCollider->getTransform().position;

					renderCircle(circleColliderWorldPos, circleCollider->getRadius(), Color(0, 0, 255), false,
								 aCurrentCamera, aScreenViewPort);
				}
			}
		}
	}
}

/**
 * @brief Retrieves the renderer.
 * @return The renderer.
 */
Renderer& RenderSystem::getRenderer() { return *mRenderer; }

/**
 * @brief Retrieves the window.
 * @return The window.
 */
Window& RenderSystem::getWindow() { return *mWindow; }

/**
 * @brief adds object to the render system.
 * @param aObject The object to add.
 */
void RenderSystem::addObject(GameObject& aObject)
{
	auto it = std::find_if(mObjects.begin(), mObjects.end(),
						   [&aObject](const std::reference_wrapper<GameObject>& wrapper)
						   {
							   return &wrapper.get() == &aObject; // Compare addresses
						   });
	if (it == mObjects.end())
	{
		// Object has not been added yet
		mObjects.push_back(aObject);
	}
}

/**
 * @brief Removes object from the render system.
 * @param aObject The object to remove.
 */
void RenderSystem::removeObject(GameObject& aObject)
{
	auto it = std::remove_if(mObjects.begin(), mObjects.end(), [&aObject](const std::reference_wrapper<GameObject>& obj)
							 { return &obj.get() == &aObject; });
	if (it != mObjects.end())
	{
		mObjects.erase(it, mObjects.end());
	}
}

/**
 * @brief Retrieves all objects in the render system.
 * @return A vector of GameObject references.
 */
const std::vector<std::reference_wrapper<GameObject>>& RenderSystem::getObjects() const { return mObjects; }

/**
 * @brief Clears all objects from the render system.
 */
void RenderSystem::clearObjects() { mObjects.clear(); }
