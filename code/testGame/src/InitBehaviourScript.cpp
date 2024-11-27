#include "InitBehaviourScript.h"

#include "AudioSource.h"
#include "BoxCollider.h"
#include "Button.h"
#include "CanvasBehaviourScript.h"
#include "EngineBravo.h"
#include "FPSCounterBehaviourScript.h"
#include "FRect.h"
#include "FSConverter.h"
#include "GameObject.h"
#include "PlayerPrefab.h"
#include "RigidBody.h"
#include "RoomBehaviourScript.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Text.h"
#include "TileMapParser.h"

SpriteDef textBackgroundDef = {"UI/ui_images.png", Rect{0, 96, 48, 32}, 48, 32};

SpriteDef guyFrameDef = {"Dungeontileset/0x72_DungeonTilesetII_v1.7.png", Rect{182, 389, 20, 27}, 20, 27};

void InitBehaviourScript::createLevel1()
{
	EngineBravo& engine = EngineBravo::getInstance();
	SceneManager& sceneManager = engine.getSceneManager();

	Scene* scene = sceneManager.createScene("Level-1");
	if (scene == nullptr)
	{
		exit(1);
	}

	Camera* camera = new Camera;
	camera->setTag("MainCamera");
	camera->setActive(true);

	camera->setTransform(Transform(Vector2(80, 96)));
	camera->setWidth(16 * 30);
	camera->setHeight(9 * 30);

	camera->setViewport(FRect{0, 0, 1, 1});

	scene->addGameObject(camera);

	Camera* miniMapCamera = new Camera;
	miniMapCamera->setTag("MiniMapCamera");
	miniMapCamera->setActive(true);

	miniMapCamera->setTransform(Transform(Vector2(80, 96)));
	miniMapCamera->setWidth(16 * 60);
	miniMapCamera->setHeight(9 * 60);

	miniMapCamera->setViewport(FRect{0.5, 0.5, 0.5, 0.5});

	scene->addGameObject(miniMapCamera);

	GameObject* defaultPlayerPrefab = PlayerPrefabFactory().createPlayerPrefab();

	CircleCollider* circleCollider = new CircleCollider(8);
	circleCollider->setTransform(Transform(Vector2(8, 0)));
	defaultPlayerPrefab->addComponent(circleCollider);

	scene->addGameObject(defaultPlayerPrefab);

	GameObject* gameObject2 = new GameObject;
	Transform objectTransform2;
	objectTransform2.position.x = 50;
	objectTransform2.position.y = 80;

	gameObject2->setTransform(objectTransform2);

	Sprite* guySprite = engine.getResourceManager().createSprite(guyFrameDef);
	guySprite->setLayer(3);
	gameObject2->addComponent(guySprite);

	gameObject2->addComponent<BoxCollider>();

	gameObject2->getComponents<BoxCollider>().at(0)->setWidth(guySprite->getWidth());
	gameObject2->getComponents<BoxCollider>().at(0)->setHeight(guySprite->getHeight());

	gameObject2->addComponent<RigidBody>();
	RigidBody* rigidBody = gameObject2->getComponents<RigidBody>().at(0);
	rigidBody->setHasGravity(true);
	rigidBody->setDensity(1.0f);
	rigidBody->setFriction(0.3f);
	rigidBody->setRestitution(0.2f);
	rigidBody->setMass(1.0f);
	rigidBody->setGravityScale(10.0f);
	rigidBody->setCanRotate(false);
	gameObject2->setName("Guy");

	scene->addGameObject(gameObject2);

	FSConverter fsConverter;
	std::string path = fsConverter.getResourcePath("LevelDefs/levelwithcollision.json");

	TileMapParser tileMapParser(path);
	tileMapParser.parse();
	const TileMapData& tileMapData = tileMapParser.getTileMapData();

	for (const auto& roomTrigger : tileMapData.mRoomTriggers)
	{
		// Collect enemy spawns for this room
		std::vector<SpawnPoint> enemySpawns;
		for (const auto& spawnPoint : tileMapData.mSpawnPoints)
		{
			if (spawnPoint.isEnemySpawn && spawnPoint.roomID == roomTrigger.roomID)
			{
				enemySpawns.push_back(spawnPoint);
			}
		}

		GameObject* roomObject = new GameObject;
		roomObject->addComponent(new RoomBehaviourScript(roomTrigger.roomID, enemySpawns));
		BoxCollider* boxCollider = new BoxCollider();
		Transform transform;
		transform.position.x = roomTrigger.x;
		transform.position.y = roomTrigger.y;
		boxCollider->setTransform(transform);
		boxCollider->setWidth(roomTrigger.mWidth);
		boxCollider->setHeight(roomTrigger.mHeight);
		boxCollider->setTrigger(true);
		roomObject->addComponent(boxCollider);
		RigidBody* rigidBody = new RigidBody();
		rigidBody->setTransform(transform);
		roomObject->addComponent(rigidBody);
		roomObject->setName("Roomtrigger");
		scene->addGameObject(roomObject);
	}

	GameObject* canvasObject = new GameObject;

	canvasObject->addComponent<CanvasBehaviourScript>();

	scene->addGameObject(canvasObject);

	GameObject* textObject =
		new Text("FPS: ", "font/SupremeSpike.otf", Color(86, 140, 100), Vector2(0, -30), Vector2(1, 1));
	Text* text = dynamic_cast<Text*>(textObject);
	text->setLayer(5);

	text->addComponent<FPSCounterBehaviourScript>();

	int textWidth = 0;
	int textHeight = 0;
	if (!engine.getRenderSystem().getTextSize(text->getFont(), text->getText(), textWidth, textHeight,
											  text->getScale()))
	{
		std::cout << "Failed to get text size for FPS counter.\n";
	}

	Sprite* textBackground = engine.getResourceManager().createSprite(textBackgroundDef);
	textBackground->setLayer(3);
	textBackground->setWidth(10);
	textBackground->setHeight(10);

	text->addComponent(textBackground);

	scene->addGameObject(text);

	GameObject* musicObject = new GameObject;

	// Add music
	AudioSource* music = new AudioSource("Audio/music.wav", true);
	music->setPlayOnWake(true);
	music->setVolume(10);
	music->setXDirection(0);
	musicObject->addComponent(music);

	scene->addGameObject(musicObject);

	sceneManager.requestSceneChange("Level-1");

	// Assuming tileMapData is a const reference to TileMapData
	for (size_t layerIndex = 0; layerIndex < tileMapData.mLayers.size(); ++layerIndex)
	{
		bool isDoorsLayer = (tileMapData.mLayerNames[layerIndex] == "Doors");
		// Access rows within the layer by index
		for (size_t rowIndex = 0; rowIndex < tileMapData.mLayers[layerIndex].size(); ++rowIndex)
		{
			// Access each tile in the row by index
			for (size_t colIndex = 0; colIndex < tileMapData.mLayers[layerIndex][rowIndex].size(); ++colIndex)
			{
				int tile = tileMapData.mLayers[layerIndex][rowIndex][colIndex];
				if (tile != 0)
				{
					// Check if the tile exists in mTileInfoMap (read-only)
					auto it = tileMapData.mTileInfoMap.find(tile);
					if (it != tileMapData.mTileInfoMap.end())
					{
						const TileInfo& tileInfo = it->second; // Access as const

						SpriteDef spriteDef = {tileInfo.mTilesetName,
											   Rect{tileInfo.mCoordinates.first, tileInfo.mCoordinates.second, 16, 16},
											   16, 16};

						GameObject* gameObject = new GameObject;

						Transform objectTransform;
						objectTransform.position.x = static_cast<int>(colIndex * 16);
						objectTransform.position.y = static_cast<int>(rowIndex * 16);
						gameObject->setTransform(objectTransform);

						// Add a Sprite component to the GameObject
						Sprite* sprite = engine.getResourceManager().createSprite(spriteDef);

						sprite->setLayer(layerIndex);

						gameObject->addComponent(sprite);

						// Add BoxCollider components to the GameObject
						for (const auto& collider : tileInfo.mColliders)
						{
							BoxCollider* boxCollider = new BoxCollider();
							Transform transform;
							transform.position.x = collider.x;
							transform.position.y = collider.y;
							boxCollider->setTransform(transform);
							boxCollider->setWidth(collider.mWidth);
							boxCollider->setHeight(collider.mHeight);
							if (isDoorsLayer)
							{
								boxCollider->setActive(false);
							}
							gameObject->addComponent(boxCollider);
						}

						if (!tileInfo.mColliders.empty())
						{
							RigidBody* rigidBody = new RigidBody();
							rigidBody->setTransform(objectTransform);
							if (isDoorsLayer)
							{
								if (rigidBody != nullptr)
								{
									rigidBody->setActive(false);
								}
							}
							gameObject->addComponent(rigidBody);
							gameObject->setName("Tile");
						}
						if (isDoorsLayer)
						{
							gameObject->setTag("Door");
						}
						scene->addGameObject(gameObject);
					}
					else
					{
						// Handle the case where tileId does not exist in the map
						std::cout << "Tile ID " << tile << " not found in mTileInfoMap.\n";
					}
				}
			}
		}
	}

	return;
}

void InitBehaviourScript::onStart() { createLevel1(); }

void InitBehaviourScript::onUpdate() {}

void InitBehaviourScript::onCollide(GameObject* aGameObject) {}

std::unique_ptr<Component> InitBehaviourScript::clone() const { return std::make_unique<InitBehaviourScript>(*this); }
