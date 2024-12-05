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

RenderSystem::RenderSystem() : WindowWidth(800), WindowHeight(450), mAspectRatio(Point{16, 9})
{
	mWindow = std::make_unique<Window>(WindowWidth, WindowHeight);
	mRenderer = std::make_unique<Renderer>(*mWindow);

	mBackgroundColor = Color(255, 255, 255);

	return;
}

void RenderSystem::setAspectRatio(const Point& aAspectRatio) { mAspectRatio = aAspectRatio; }

Point RenderSystem::getAspectRatio() const { return mAspectRatio; }

void RenderSystem::renderSprite(const Camera& aCurrentCamera, const GameObject& aGameObject, const Sprite& aSprite,
								const Rect& aScreenViewPort) const
{
	Vector2 texturePosition = aGameObject.getTransform().position + aSprite.getRelativePosition().position;
	Vector2 cameraOrigin = aCurrentCamera.getOrigin();
	Vector2 drawPosition = texturePosition - cameraOrigin;

	// SDL_Rect cameraRect = {static_cast<int>(cameraOrigin.x), static_cast<int>(cameraOrigin.y),
	// 					   static_cast<int>(aCurrentCamera.getWidth()), static_cast<int>(aCurrentCamera.getHeight())};
	//
	// SDL_Rect spriteRect = {static_cast<int>(drawPosition.x), static_cast<int>(drawPosition.y), aSprite->getWidth(),
	// 					   aSprite->getHeight()};

	// // Perform culling: check if the sprite is outside the camera's view
	// if (!SDL_HasIntersection(&cameraRect, &spriteRect))
	// {
	// 	return; // Skip rendering
	// }

	// Adjust draw position and size to the viewport
	drawPosition.x = std::round(drawPosition.x * ((float)aScreenViewPort.w / aCurrentCamera.getWidth()));
	drawPosition.y = std::round(drawPosition.y * ((float)aScreenViewPort.h / aCurrentCamera.getHeight()));

	int spriteWidth = std::round(aSprite.getWidth() * ((float)aScreenViewPort.w / aCurrentCamera.getWidth())) + 1;
	int spriteHeight = std::round(aSprite.getHeight() * ((float)aScreenViewPort.h / aCurrentCamera.getHeight())) + 1;

	// Render
	mRenderer->renderTexture(*aSprite.getTexture(), aSprite.getSource(), drawPosition, spriteWidth, spriteHeight,
							 aSprite.getFlipX(), aSprite.getFlipY(),
							 aGameObject.getTransform().rotation + aSprite.getRelativePosition().rotation,
							 aSprite.getColorFilter());
}

void RenderSystem::renderAnimation(const Camera& aCurrentCamera, const GameObject& aGameObject,
								   const Animation& aAnimation, const Rect& aScreenViewPort) const
{
	const Sprite& currentFrame = aAnimation.getCurrentFrame();

	renderSprite(aCurrentCamera, aGameObject, currentFrame, aScreenViewPort);
}

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

void RenderSystem::renderLayer(const Scene& aScene, int aLayer, const Camera& activeCamera,
							   const Rect& aScreenViewPort) const
{
	for (GameObject& gameObject : mObjects)
	{
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

void RenderSystem::sortCamerasByRenderOrder(std::vector<Camera*>& aCameras) const
{
	std::sort(aCameras.begin(), aCameras.end(),
			  [](Camera* a, Camera* b) { return a->getRenderOrder() < b->getRenderOrder(); });
}

void RenderSystem::render(const Scene& aScene) const
{
	mRenderer->clear(mBackgroundColor);

	// Get all active cameras
	std::vector<Camera*> cameras = aScene.getCameras();

	// Sort cameras by render order
	sortCamerasByRenderOrder(cameras);

	for (Camera* camera : cameras)
	{

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

void RenderSystem::renderSquare(const Vector2& aPosition, const int aWidth, const int aHeight, const float aRotation,
								const Color& aColor, bool aFilled, const Camera& aCurrentCamera,
								const Rect& aScreenViewPort) const
{

	Vector2 squarePosition = aPosition;
	Vector2 cameraOrigin = aCurrentCamera.getOrigin();
	Vector2 drawPosition = squarePosition - cameraOrigin;

	// Adjust draw position and size to the viewport
	drawPosition.x = std::round(drawPosition.x * ((float)aScreenViewPort.w / aCurrentCamera.getWidth()));
	drawPosition.y = std::round(drawPosition.y * ((float)aScreenViewPort.h / aCurrentCamera.getHeight()));

	int squareWidth = std::round(aWidth * ((float)aScreenViewPort.w / aCurrentCamera.getWidth())) + 1;
	int squareHeight = std::round(aHeight * ((float)aScreenViewPort.h / aCurrentCamera.getHeight())) + 1;

	// Render
	mRenderer->renderSquare(drawPosition, squareWidth, squareHeight, aRotation, aColor, aFilled);
}

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
					Vector2 boxColliderWorldPos =
						gameObject->getTransform().position + boxCollider->getTransform().position;

					renderSquare(boxColliderWorldPos, boxCollider->getWidth(), boxCollider->getHeight(),
								 gameObject->getTransform().rotation, Color(0, 0, 255), false, aCurrentCamera,
								 aScreenViewPort);
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

Renderer& RenderSystem::getRenderer() { return *mRenderer; }

Window& RenderSystem::getWindow() { return *mWindow; }

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

void RenderSystem::removeObject(GameObject& aObject)
{
	auto it = std::remove_if(mObjects.begin(), mObjects.end(), [&aObject](const std::reference_wrapper<GameObject>& obj)
							 { return &obj.get() == &aObject; });
	if (it != mObjects.end())
	{
		mObjects.erase(it, mObjects.end());
	}
}

const std::vector<std::reference_wrapper<GameObject>>& RenderSystem::getObjects() const { return mObjects; }

void RenderSystem::clearObjects() { mObjects.clear(); }
