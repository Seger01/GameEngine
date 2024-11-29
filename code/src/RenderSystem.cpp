#include "RenderSystem.h"

#include <iostream>
#include <memory>

#include "Animation.h"
#include "BoxCollider.h"
#include "Button.h"
#include "Color.h"
#include "EngineBravo.h"
#include "ParticleEmitter.h"
#include "SDL_timer.h"
#include "ScopedTimer.h"
#include "Sprite.h"
#include "Text.h"
#include "Time.h"
#include "UIObject.h"

RenderSystem::RenderSystem() : WindowWidth(800), WindowHeight(480)
{
	mWindow = std::make_unique<Window>(WindowWidth, WindowHeight);
	mRenderer = std::make_unique<Renderer>(*mWindow);

	mBackgroundColor = Color(255, 255, 255);

	return;
}

// void RenderSystem::renderSprite(Camera& aCurrentCamera, GameObject* aGameObject, Sprite* aSprite)
// {
//
// 	int spriteWidth = aSprite->getWidth();
// 	int spriteHeight = aSprite->getHeight();
//
// 	int windowWidth = mWindow->getSize().x;
// 	int windowHeight = mWindow->getSize().y;
//
// 	Vector2 texturePosition = aGameObject->getTransform().position + aSprite->getRelativePosition().position;
//
// 	// Calculate the camera's origin and position the sprite relative to it
// 	Vector2 cameraOrigin = aCurrentCamera.getOrigin();
//
// 	Vector2 drawPosition = texturePosition - cameraOrigin;
//
// 	// Snap position to integer to avoid subpixel offsets
// 	drawPosition.x = std::round(drawPosition.x * (static_cast<float>(windowWidth) / aCurrentCamera.getWidth()));
// 	drawPosition.y = std::round(drawPosition.y * (static_cast<float>(windowHeight) / aCurrentCamera.getHeight()));
//
// 	// Adjust the width and height slightly to cover gaps
// 	spriteWidth = std::round(spriteWidth * (static_cast<float>(windowWidth) / aCurrentCamera.getWidth())) + 1;
// 	spriteHeight = std::round(spriteHeight * (static_cast<float>(windowHeight) / aCurrentCamera.getHeight())) + 1;
//
// 	// Render the sprite with adjusted size
// 	mRenderer->renderTexture(*aSprite->getTexture(), aSprite->getSource(), drawPosition, spriteWidth, spriteHeight,
// 							 aSprite->getFlipX(), aSprite->getFlipY(),
// 							 aGameObject->getTransform().rotation + aSprite->getRelativePosition().rotation,
// 							 aSprite->getColorFilter());
// }

void RenderSystem::renderSprite(Camera& aCurrentCamera, GameObject* aGameObject, Sprite* aSprite, Rect aScreenViewPort)
{
	Vector2 texturePosition = aGameObject->getTransform().position + aSprite->getRelativePosition().position;
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

	// drawPosition.x += aScreenViewPort.x;
	// drawPosition.y += aScreenViewPort.y;

	int spriteWidth = std::round(aSprite->getWidth() * ((float)aScreenViewPort.w / aCurrentCamera.getWidth())) + 1;
	int spriteHeight = std::round(aSprite->getHeight() * ((float)aScreenViewPort.h / aCurrentCamera.getHeight())) + 1;

	// Render
	mRenderer->renderTexture(*aSprite->getTexture(), aSprite->getSource(), drawPosition, spriteWidth, spriteHeight,
							 aSprite->getFlipX(), aSprite->getFlipY(),
							 aGameObject->getTransform().rotation + aSprite->getRelativePosition().rotation,
							 aSprite->getColorFilter());
}

void RenderSystem::renderAnimation(Camera& aCurrentCamera, GameObject* aGameObject, Animation* aAnimation,
								   Rect aScreenViewPort)
{
	Sprite* currentFrame = aAnimation->getCurrentFrame();
	// Sprite* currentFrame = aAnimation->getFrame(0);

	renderSprite(aCurrentCamera, aGameObject, currentFrame, aScreenViewPort);
}

void RenderSystem::renderParticle(Camera& aCurrentCamera, Particle& aParticle, Rect aScreenViewPort)
{
	float particleWidth = aParticle.getSize().x;
	float particleHeight = aParticle.getSize().y;

	Vector2 particlePosition = aParticle.getPosition();
	Vector2 cameraOrigin = aCurrentCamera.getOrigin();
	Vector2 drawPosition = particlePosition - cameraOrigin;

	drawPosition.x = drawPosition.x * (static_cast<float>(aScreenViewPort.w) / aCurrentCamera.getWidth());
	drawPosition.y = drawPosition.y * (static_cast<float>(aScreenViewPort.h) / aCurrentCamera.getHeight());

	// drawPosition.x += aScreenViewPort.x;
	// drawPosition.y += aScreenViewPort.y;

	particleWidth =
		static_cast<int>(static_cast<float>(particleWidth) *
						 (static_cast<float>(aScreenViewPort.w) / static_cast<float>(aCurrentCamera.getWidth())));
	particleHeight =
		static_cast<int>(static_cast<float>(particleHeight) *
						 (static_cast<float>(aScreenViewPort.h) / static_cast<float>(aCurrentCamera.getHeight())));

	if (aParticle.getRotation() == 0)
	{
		mRenderer->renderSquare(drawPosition, static_cast<int>(particleWidth), static_cast<int>(particleHeight),
								aParticle.getColor(), true);
	}
	else
	{
		mRenderer->renderSquare(drawPosition, static_cast<int>(particleWidth), static_cast<int>(particleHeight),
								aParticle.getRotation(), aParticle.getColor(), true);
	}
}

void RenderSystem::renderText(Camera& aCurrentCamera, const std::string& aText, Vector2 aLocation, Color aColor,
							  Vector2 aScale, Rect aScreenViewPort)
{
	float scaleX = aScale.x * (aScreenViewPort.w / static_cast<float>(aCurrentCamera.getWidth()));
	float scaleY = aScale.y * (aScreenViewPort.h / static_cast<float>(aCurrentCamera.getHeight()));

	Vector2 cameraOrigin = aCurrentCamera.getOrigin();
	Vector2 drawPosition = aLocation - cameraOrigin;

	drawPosition.x = drawPosition.x * (static_cast<float>(aScreenViewPort.w) / aCurrentCamera.getWidth());
	drawPosition.y = drawPosition.y * (static_cast<float>(aScreenViewPort.h) / aCurrentCamera.getHeight());

	// drawPosition.x += aScreenViewPort.x;
	// drawPosition.y += aScreenViewPort.y;

	mRenderer->renderText(aText, drawPosition, aColor, scaleX, scaleY);
}

bool RenderSystem::getTextSize(const std::string& aFont, const std::string& aText, int& aWidth, int& aHeight,
							   Vector2 aScale)
{
	if (!mRenderer->calculateTextSize(aFont, aText, aWidth, aHeight))
	{
		return false;
	}

	aWidth = aWidth * aScale.x;
	aHeight = aHeight * aScale.y;

	return true;
}

Vector2 RenderSystem::screenToWorldPos(Point aScreenpos, Camera& aCurrentCamera)
{
	Vector2 screenPos{static_cast<float>(aScreenpos.x), static_cast<float>(aScreenpos.y)};
	FRect viewport = aCurrentCamera.getViewport();

	Rect screenViewPort =
		Rect{static_cast<int>(viewport.x * mWindow->getSize().x), static_cast<int>(viewport.y * mWindow->getSize().y),
			 static_cast<int>(viewport.w * mWindow->getSize().x), static_cast<int>(viewport.h * mWindow->getSize().y)};

	screenPos.x = screenPos.x - screenViewPort.x;
	screenPos.y = screenPos.y - screenViewPort.y;

	Vector2 worldPos;
	worldPos.x = screenPos.x * (aCurrentCamera.getWidth() / (viewport.w * mWindow->getSize().x));
	worldPos.y = screenPos.y * (aCurrentCamera.getHeight() / (viewport.h * mWindow->getSize().y));

	worldPos.x = worldPos.x + aCurrentCamera.getOrigin().x;
	worldPos.y = worldPos.y + aCurrentCamera.getOrigin().y;

	return worldPos;
}

int RenderSystem::getLowestLayer(Scene* aScene)
{
	int lowestLayer = 0;
	for (auto& gameObject : aScene->getGameObjects())
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

int RenderSystem::getHighestLayer(Scene* aScene)
{
	int highestLayer = 0;
	for (auto& gameObject : aScene->getGameObjects())
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

void RenderSystem::renderLayer(Scene* aScene, int aLayer, Camera& activeCamera, Rect aScreenViewPort)
{
	for (GameObject& gameObject : mObjects)
	{
		for (auto animation : gameObject.getComponents<Animation>())
		{
			if (animation->isActive() && animation->getLayer() == aLayer)
			{
				renderAnimation(activeCamera, &gameObject, animation, aScreenViewPort);
			}
		}
		for (auto sprite : gameObject.getComponents<Sprite>())
		{
			if (sprite->isActive() && sprite->getLayer() == aLayer)
			{
				renderSprite(activeCamera, &gameObject, sprite, aScreenViewPort);
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

void RenderSystem::render(Scene* aScene)
{
	mRenderer->clear(mBackgroundColor);

	// Get all active cameras
	std::vector<Camera*> cameras = aScene->getCameras();

	for (Camera* camera : cameras)
	{

		FRect viewport = camera->getViewport();

		Rect screenViewPort = Rect{
			static_cast<int>(viewport.x * mWindow->getSize().x), static_cast<int>(viewport.y * mWindow->getSize().y),
			static_cast<int>(viewport.w * mWindow->getSize().x), static_cast<int>(viewport.h * mWindow->getSize().y)};

		mRenderer->setViewport(screenViewPort);

		// Render objects visible to this camera
		renderForCamera(aScene, *camera, screenViewPort);
	}

	mRenderer->show();
}

void RenderSystem::renderForCamera(Scene* aScene, Camera& camera, Rect aScreenViewPort)
{
	int lowestLayer = getLowestLayer(aScene);
	int highestLayer = getHighestLayer(aScene);

	for (int layer = lowestLayer; layer <= highestLayer; ++layer)
	{
		renderLayer(aScene, layer, camera, aScreenViewPort);
	}

	renderDebugInfo(aScene, camera);
}

void RenderSystem::renderDebugInfo(Scene* aScene, Camera& aCurrentCamera)
{
	if (Time::deltaTime == 0)
	{
		return;
	}

	Configuration& config = EngineBravo::getInstance().getConfiguration();

	if (config.getConfig(SHOW_FPS))
	{
		int fps = 1.0f / Time::deltaTime;

		mRenderer->renderText("FPS: " + std::to_string(fps), Vector2(10, 10), Color(0, 255, 0), 1.5, 1.5);
	}

	if (config.getConfig(SHOW_COLLIDERS))
	{
		for (auto& gameObject : aScene->getGameObjects())
		{
			if (gameObject->hasComponent<BoxCollider>())
			{
				for (auto boxCollider : gameObject->getComponents<BoxCollider>())
				{
					int spriteWidth = boxCollider->getWidth();
					int spriteHeight = boxCollider->getHeight();

					int WindowWidth = mWindow->getSize().x;
					int WindowHeight = mWindow->getSize().y;

					Vector2 texturePosition =
						gameObject->getTransform().position + boxCollider->getTransform().position;

					Vector2 cameraOrigin = aCurrentCamera.getTransform().position -
										   Vector2(aCurrentCamera.getWidth() / 2.0f, aCurrentCamera.getHeight() / 2.0f);

					Vector2 drawPosition = texturePosition - cameraOrigin;

					drawPosition.x = drawPosition.x * (static_cast<float>(WindowWidth) / aCurrentCamera.getWidth());
					drawPosition.y = drawPosition.y * (static_cast<float>(WindowHeight) / aCurrentCamera.getHeight());

					spriteWidth = std::ceil(static_cast<int>(
						static_cast<float>(spriteWidth) *
						(static_cast<float>(WindowWidth) / static_cast<float>(aCurrentCamera.getWidth()))));
					spriteHeight = std::ceil(static_cast<int>(
						static_cast<float>(spriteHeight) *
						(static_cast<float>(WindowHeight) / static_cast<float>(aCurrentCamera.getHeight()))));

					Color renderColor = Color(0, 0, 255);

					if (!boxCollider->isActive())
					{
						renderColor = Color(252, 3, 252);
					}

					mRenderer->renderSquare(drawPosition, spriteWidth, spriteHeight, renderColor, false);
				}
			}
			if (gameObject->hasComponent<CircleCollider>())
			{
				for (auto circleCollider : gameObject->getComponents<CircleCollider>())
				{
					int WindowWidth = mWindow->getSize().x;
					int WindowHeight = mWindow->getSize().y;

					Vector2 circlePos = gameObject->getTransform().position + circleCollider->getTransform().position;

					float circleRadius = circleCollider->getRadius();

					Vector2 cameraOrigin = aCurrentCamera.getOrigin();

					Vector2 drawPosition = circlePos - cameraOrigin;

					drawPosition.x = drawPosition.x * (static_cast<float>(WindowWidth) / aCurrentCamera.getWidth());
					drawPosition.y = drawPosition.y * (static_cast<float>(WindowHeight) / aCurrentCamera.getHeight());

					circleRadius = std::ceil(static_cast<int>(
						static_cast<float>(circleRadius) *
						(static_cast<float>(WindowWidth) / static_cast<float>(aCurrentCamera.getWidth()))));

					Color renderColor = Color(0, 0, 255);

					if (!circleCollider->isActive())
					{
						renderColor = Color(252, 3, 252);
					}

					mRenderer->drawCircle(drawPosition, circleRadius, renderColor, false);
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
