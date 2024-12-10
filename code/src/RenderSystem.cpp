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

b2DebugDraw debugDraw;
RenderSystem* renderSystem;
Renderer* renderer;

void DrawPolygonImpl(const b2Vec2* vertices, int vertexCount, b2HexColor color, void* context)
{
	// Convert color from b2HexColor to Renderer Color
	Color renderColor{(Uint8)((color >> 16) & 0xFF), (Uint8)((color >> 8) & 0xFF), (Uint8)(color & 0xFF), 255};

	// Draw edges between consecutive vertices
	for (int i = 0; i < vertexCount; ++i)
	{
		Vector2 start(vertices[i].x, vertices[i].y);
		Vector2 end(vertices[(i + 1) % vertexCount].x, vertices[(i + 1) % vertexCount].y);

		// renderer->renderLine(start, end, renderColor);
	}
}

// void DrawSolidPolygonImpl(b2Transform transform, const b2Vec2* vertices, int vertexCount, float radius,
// 						  b2HexColor color, void* context)
// {
// 	std::cout << "DrawSolidPolygonImpl" << std::endl;
// 	// Assume solid polygon is a rectangle for this implementation
// 	if (vertexCount == 4) // Check if it's a rectangle
// 	{
// 		Vector2 location(vertices[0].x, vertices[0].y); // Top-left corner
// 		float width = vertices[2].x - vertices[0].x;
// 		float height = vertices[2].y - vertices[0].y;
//
// 		// Convert color from b2HexColor to Renderer Color
// 		Color renderColor{(Uint8)((color >> 16) & 0xFF), (Uint8)((color >> 8) & 0xFF), (Uint8)(color & 0xFF), 255};
//
// 		location.x = location.x + transform.p.x;
// 		location.y = location.y + transform.p.y;
//
// 		// Render the rectangle
// 		std::cout << "Render Pos: " << location.x << ", " << location.y << std::endl;
// 		renderer->renderSquare(location, static_cast<int>(width), static_cast<int>(height), 0.0f, renderColor, true);
// 	}
// 	else
// 	{
// 		std::cout << "gotten polygon not a rectangle" << std::endl;
// 	}
// }
void DrawSolidPolygonImpl(b2Transform transform, const b2Vec2* vertices, int vertexCount, float radius,
						  b2HexColor color, void* context)
{
	// Convert color from b2HexColor to Renderer Color
	Color renderColor{(Uint8)((color >> 16) & 0xFF), (Uint8)((color >> 8) & 0xFF), (Uint8)(color & 0xFF), 255};

	// Create a vector to store transformed vertices
	std::vector<Vector2> transformedVertices;
	transformedVertices.reserve(vertexCount);

	// Transform all vertices by the b2Transform
	for (int i = 0; i < vertexCount; ++i)
	{
		// Apply the transform to each vertex
		b2Vec2 transformedVertex = b2Mul(transform.p, vertices[i]);
		transformedVertices.emplace_back(transformedVertex.x, transformedVertex.y);
	}

	// Render the polygon using the renderer
	renderer->renderPolygon(transformedVertices, renderColor, true);
}

void DrawCircleImpl(b2Vec2 center, float radius, b2HexColor color, void* context)
{
	// Convert color from b2HexColor to Renderer Color
	Color renderColor{(Uint8)((color >> 16) & 0xFF), (Uint8)((color >> 8) & 0xFF), (Uint8)(color & 0xFF), 255};

	// Render the circle outline
	renderer->drawCircle(Vector2(center.x, center.y), static_cast<int>(radius), renderColor, false);
}

void DrawSolidCircleImpl(b2Transform transform, float radius, b2HexColor color, void* context)
{
	// Convert color from b2HexColor to Renderer Color
	Color renderColor{(Uint8)((color >> 16) & 0xFF), (Uint8)((color >> 8) & 0xFF), (Uint8)(color & 0xFF), 255};

	// Use the position from the transform
	Vector2 center(transform.p.x, transform.p.y);

	// Render the filled circle
	renderer->drawCircle(center, static_cast<int>(radius), renderColor, true);
}

// void DrawPolygonImpl(const b2Vec2* vertices, int vertexCount, b2HexColor color, void* context) {}
//
// void DrawSolidPolygonImpl(b2Transform transform, const b2Vec2* vertices, int vertexCount, float radius,
// 						  b2HexColor color, void* context)
// {
// }
//
// void DrawCircleImpl(b2Vec2 center, float radius, b2HexColor color, void* context) {}
//
// void DrawSolidCircleImpl(b2Transform transform, float radius, b2HexColor color, void* context) {}

// void DrawPolygonImpl(const b2Vec2* vertices, int vertexCount, b2HexColor color, void* context)
// {
// 	Renderer* renderer = static_cast<Renderer*>(context);
// 	if (!renderer || vertexCount < 2)
// 		return;
//
// 	Color renderColor{(Uint8)((color >> 16) & 0xFF), (Uint8)((color >> 8) & 0xFF), (Uint8)(color & 0xFF), 255};
//
// 	SDL_SetRenderDrawColor(renderer->getSDLRenderer(), renderColor.r, renderColor.g, renderColor.b, 255);
//
// 	for (int i = 0; i < vertexCount; ++i)
// 	{
// 		const b2Vec2& start = vertices[i];
// 		const b2Vec2& end = vertices[(i + 1) % vertexCount]; // Wrap around to the first vertex
// 		SDL_RenderDrawLine(renderer->getSDLRenderer(), static_cast<int>(start.x), static_cast<int>(start.y),
// 						   static_cast<int>(end.x), static_cast<int>(end.y));
// 	}
// }
//
// void DrawSolidPolygonImpl(b2Transform transform, const b2Vec2* vertices, int vertexCount, float radius,
// 						  b2HexColor color, void* context)
// {
// 	Renderer* renderer = static_cast<Renderer*>(context);
// 	if (!renderer || vertexCount < 3)
// 		return;
//
// 	Color renderColor{(Uint8)((color >> 16) & 0xFF), (Uint8)((color >> 8) & 0xFF), (Uint8)(color & 0xFF), 255};
// 	SDL_SetRenderDrawColor(renderer->getSDLRenderer(), renderColor.r, renderColor.g, renderColor.b, 255);
//
// 	// Use a triangulation method to draw a filled polygon
// 	for (int i = 1; i < vertexCount - 1; ++i)
// 	{
// 		SDL_Point points[3] = {{static_cast<int>(vertices[0].x + transform.p.x), static_cast<int>(vertices[0].y)},
// 							   {static_cast<int>(vertices[i].x + transform.p.x), static_cast<int>(vertices[i].y)},
// 							   {static_cast<int>(vertices[i + 1].x), static_cast<int>(vertices[i + 1].y)}};
// 		SDL_RenderDrawLines(renderer->getSDLRenderer(), points, 3);
// 	}
// }
//
// void DrawCircleImpl(b2Vec2 center, float radius, b2HexColor color, void* context)
// {
// 	Renderer* renderer = static_cast<Renderer*>(context);
// 	if (!renderer)
// 		return;
//
// 	Color renderColor{(Uint8)((color >> 16) & 0xFF), (Uint8)((color >> 8) & 0xFF), (Uint8)(color & 0xFF), 255};
// 	SDL_SetRenderDrawColor(renderer->getSDLRenderer(), renderColor.r, renderColor.g, renderColor.b, 255);
//
// 	int x = static_cast<int>(radius);
// 	int y = 0;
// 	int decisionOver2 = 1 - x; // Decision variable for the midpoint circle algorithm
//
// 	while (x >= y)
// 	{
// 		// Draw the circle's eight symmetrical points
// 		SDL_RenderDrawPoint(renderer->getSDLRenderer(), center.x + x, center.y + y);
// 		SDL_RenderDrawPoint(renderer->getSDLRenderer(), center.x + y, center.y + x);
// 		SDL_RenderDrawPoint(renderer->getSDLRenderer(), center.x - y, center.y + x);
// 		SDL_RenderDrawPoint(renderer->getSDLRenderer(), center.x - x, center.y + y);
// 		SDL_RenderDrawPoint(renderer->getSDLRenderer(), center.x - x, center.y - y);
// 		SDL_RenderDrawPoint(renderer->getSDLRenderer(), center.x - y, center.y - x);
// 		SDL_RenderDrawPoint(renderer->getSDLRenderer(), center.x + y, center.y - x);
// 		SDL_RenderDrawPoint(renderer->getSDLRenderer(), center.x + x, center.y - y);
//
// 		y++;
// 		if (decisionOver2 <= 0)
// 		{
// 			decisionOver2 += 2 * y + 1; // Update decision variable
// 		}
// 		else
// 		{
// 			x--;
// 			decisionOver2 += 2 * (y - x) + 1;
// 		}
// 	}
// }
//
// void DrawSolidCircleImpl(b2Transform transform, float radius, b2HexColor color, void* context)
// {
// 	Renderer* renderer = static_cast<Renderer*>(context);
// 	if (!renderer)
// 		return;
//
// 	Color renderColor{(Uint8)((color >> 16) & 0xFF), (Uint8)((color >> 8) & 0xFF), (Uint8)(color & 0xFF), 255};
// 	SDL_SetRenderDrawColor(renderer->getSDLRenderer(), renderColor.r, renderColor.g, renderColor.b, 255);
//
// 	b2Vec2 center = transform.p; // Assuming `transform.p` is the circle center
//
// 	for (int w = -static_cast<int>(radius); w <= static_cast<int>(radius); ++w)
// 	{
// 		for (int h = -static_cast<int>(radius); h <= static_cast<int>(radius); ++h)
// 		{
// 			if (w * w + h * h <= radius * radius)
// 			{
// 				SDL_RenderDrawPoint(renderer->getSDLRenderer(), center.x + w, center.y + h);
// 			}
// 		}
// 	}
// }

// Add more callback implementations as needed...

// Initialize the b2DebugDraw struct
void InitializeDebugDraw(b2DebugDraw* debugDraw, void* userContext)
{
	debugDraw->DrawPolygon = DrawPolygonImpl;
	debugDraw->DrawSolidPolygon = DrawSolidPolygonImpl;
	debugDraw->DrawCircle = DrawCircleImpl;
	debugDraw->DrawSolidCircle = DrawSolidCircleImpl;

	// Assign other function pointers similarly...

	// Initialize options
	debugDraw->useDrawingBounds = false;
	debugDraw->drawShapes = true;
	debugDraw->drawJoints = true;
	debugDraw->drawJointExtras = false;
	debugDraw->drawAABBs = false;
	debugDraw->drawMass = false;
	debugDraw->drawContacts = false;
	debugDraw->drawGraphColors = false;
	debugDraw->drawContactNormals = false;
	debugDraw->drawContactImpulses = false;
	debugDraw->drawFrictionImpulses = false;

	// Initialize context
	debugDraw->context = userContext;
}

RenderSystem::RenderSystem() : WindowWidth(800), WindowHeight(450), mAspectRatio(Point{16, 9})
{
	mWindow = std::make_unique<Window>(WindowWidth, WindowHeight);
	mRenderer = std::make_unique<Renderer>(*mWindow);

	mBackgroundColor = Color(255, 255, 255);

	// Initialize the debug draw
	InitializeDebugDraw(&debugDraw, this);
	renderSystem = this;
	renderer = mRenderer.get();

	return;
}

void RenderSystem::setAspectRatio(Point aAspectRatio) { mAspectRatio = aAspectRatio; }

Point RenderSystem::getAspectRatio() { return mAspectRatio; }

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
	Sprite& currentFrame = aAnimation->getCurrentFrame();

	renderSprite(aCurrentCamera, aGameObject, &currentFrame, aScreenViewPort);
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

	particleWidth =
		static_cast<int>(static_cast<float>(particleWidth) *
						 (static_cast<float>(aScreenViewPort.w) / static_cast<float>(aCurrentCamera.getWidth())));
	particleHeight =
		static_cast<int>(static_cast<float>(particleHeight) *
						 (static_cast<float>(aScreenViewPort.h) / static_cast<float>(aCurrentCamera.getHeight())));

	mRenderer->renderSquare(drawPosition, static_cast<int>(particleWidth), static_cast<int>(particleHeight),
							aParticle.getRotation(), aParticle.getColor(), true);
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

void RenderSystem::sortCamerasByRenderOrder(std::vector<Camera*>& aCameras)
{
	std::sort(aCameras.begin(), aCameras.end(),
			  [](Camera* a, Camera* b) { return a->getRenderOrder() < b->getRenderOrder(); });
}

void RenderSystem::render(Scene* aScene)
{
	mRenderer->clear(mBackgroundColor);

	// Get all active cameras
	std::vector<Camera*> cameras = aScene->getCameras();

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

	// b2WorldId worldId = EngineBravo::getInstance().getPhysicsManager().getPhysicsEngine().getWorld().getWorldID();
	//
	// b2World_Draw(worldId, &debugDraw);

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

	renderDebugInfo(aScene, camera, aScreenViewPort);
}

void RenderSystem::renderSquare(Vector2 aPosition, int aWidth, int aHeight, float aRotation, Color aColor, bool aFilled,
								Camera& aCurrentCamera, Rect aScreenViewPort, Point aRotationalCenter)
{

	std::cout << "Received Position: " << aPosition.x << ", " << aPosition.y << std::endl;
	std::cout << "Received Width: " << aWidth << std::endl;
	std::cout << "Received Height: " << aHeight << std::endl;
	std::cout << "Received Rotation: " << aRotation << std::endl;
	// std::cout << "Received Color: " << aColor.r << ", " << aColor.g << ", " << aColor.b << std::endl;
	// std::cout << "Received Filled: " << aFilled << std::endl;
	// std::cout << "Received Camera: " << aCurrentCamera.getOrigin().x << ", " << aCurrentCamera.getOrigin().y <<
	// std::endl; std::cout << "Received Screen Viewport: " << aScreenViewPort.x << ", " << aScreenViewPort.y << ", " <<
	// aScreenViewPort.w << ", " << aScreenViewPort.h << std::endl;
	std::cout << "Received Rotational Center: " << aRotationalCenter.x << ", " << aRotationalCenter.y << std::endl;

	Vector2 squarePosition = aPosition;
	Vector2 cameraOrigin = aCurrentCamera.getOrigin();
	Vector2 drawPosition = squarePosition - cameraOrigin;

	// Adjust draw position and size to the viewport
	drawPosition.x = std::round(drawPosition.x * ((float)aScreenViewPort.w / aCurrentCamera.getWidth()));
	drawPosition.y = std::round(drawPosition.y * ((float)aScreenViewPort.h / aCurrentCamera.getHeight()));

	int squareWidth = std::round(aWidth * ((float)aScreenViewPort.w / aCurrentCamera.getWidth())) + 1;
	int squareHeight = std::round(aHeight * ((float)aScreenViewPort.h / aCurrentCamera.getHeight())) + 1;

	aRotationalCenter.x = std::round(aRotationalCenter.x * ((float)aScreenViewPort.w / aCurrentCamera.getWidth()));
	aRotationalCenter.y = std::round(aRotationalCenter.y * ((float)aScreenViewPort.h / aCurrentCamera.getHeight()));

	std::cout << "Calculated position: " << drawPosition.x << ", " << drawPosition.y << std::endl;
	std::cout << "Calculated width: " << squareWidth << std::endl;
	std::cout << "Calculated height: " << squareHeight << std::endl;
	std::cout << "Calculated rotation: " << aRotation << std::endl;
	std::cout << "Calculated rotational center: " << aRotationalCenter.x << ", " << aRotationalCenter.y << std::endl;

	// Render
	mRenderer->renderSquare(drawPosition, squareWidth, squareHeight, aRotation, aColor, aFilled, aRotationalCenter);
}

void RenderSystem::renderCircle(Vector2 aPosition, float aRadius, Color aColor, bool aFilled, Camera& aCurrentCamera,
								Rect aScreenViewPort)
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

void RenderSystem::renderDebugInfo(Scene* aScene, Camera& aCurrentCamera, Rect aScreenViewPort)
{
	if (Time::deltaTime == 0)
	{
		return;
	}

	if (aCurrentCamera.getDebugOverlayRef().showFPS)
	{
		int fps = 1.0f / Time::deltaTime;

		renderText(aCurrentCamera, "FPS: " + std::to_string(fps),
				   Vector2(aCurrentCamera.getOrigin().x + 5, aCurrentCamera.getOrigin().y + 5), Color(0, 255, 0),
				   Vector2(0.5, 0.5), aScreenViewPort);
	}

	if (aCurrentCamera.getDebugOverlayRef().renderCameraViewport)
	{
		mRenderer->renderSquare(Vector2(0, 0), aScreenViewPort.w, aScreenViewPort.h, 0, Color(0, 255, 208), false);
	}

	if (aCurrentCamera.getDebugOverlayRef().renderColliders)
	{
		for (auto& gameObject : aScene->getGameObjects())
		{
			if (gameObject->hasComponent<BoxCollider>())
			{
				int counter = 0;
				for (auto boxCollider : gameObject->getComponents<BoxCollider>())
				{
					Vector2 relativeBoxPosition = boxCollider->getTransform().position;

					Vector2 boxColliderWorldPos =
						gameObject->getTransform().position + boxCollider->getTransform().position;

					std::cout << "square: " << counter << std::endl;
					counter++;
					renderSquare(
						boxColliderWorldPos, boxCollider->getWidth(), boxCollider->getHeight(),
						gameObject->getTransform().rotation, Color(0, 0, 255), false, aCurrentCamera, aScreenViewPort,
						Point{static_cast<int>(-relativeBoxPosition.x), static_cast<int>(-relativeBoxPosition.y)});
					// renderSquare(boxColliderWorldPos, boxCollider->getWidth(), boxCollider->getHeight(),
					// 			 gameObject->getTransform().rotation, Color(0, 0, 255), false, aCurrentCamera,
					// 			 aScreenViewPort, Point{0, 0});
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
