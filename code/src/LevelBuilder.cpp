/**
 * @file LevelBuilder.cpp
 * @brief This file contains the implementation of the LevelBuilder class
 */

#include "LevelBuilder.h"
#include "AudioSource.h"
#include "BoxCollider.h"
#include "EngineBravo.h"
#include "RigidBody.h"
#include "Scene.h"
#include "Sprite.h"

/**
 * @brief This function creates a level from the parsed json data
 * @details To use room_entry and LevelEndTrigger, the type property must be set as room_entry or LevelEndTrigger
 * respectively in the json file
 * @param scene
 * @param tileMapData
 * @throw runtime_error if scene is null
 */
void LevelBuilder::createLevel(Scene& aScene, const TileMapData& aTileMapData, int aTileWidth, int aTileHeight) const
{
	if (&aScene == nullptr)
	{
		throw std::runtime_error("Scene is null in LevelBuilder::createLevel");
	}

	EngineBravo& engine = EngineBravo::getInstance();

	createTileLayers(aScene, aTileMapData, aTileWidth, aTileHeight);
}

/**
 * @brief This function creates the tile layers
 *
 * @param scene
 * @param tileMapData
 * @throw runtime_error if tile not found in tileInfoMap
 */
void LevelBuilder::createTileLayers(Scene& aScene, const TileMapData& aTileMapData, int aTileWidth, int aTileHeight) const
{
	for (size_t layerIndex = 0; layerIndex < aTileMapData.mLayers.size(); ++layerIndex)
	{
		bool isGraphLayer = false;

		auto layerPropertiesIt = aTileMapData.mLayerProperties.find(aTileMapData.mLayerNames[layerIndex]);
		if (layerPropertiesIt != aTileMapData.mLayerProperties.end())
		{
			auto propertyIt = layerPropertiesIt->second.find("isGraphLayer");
			if (propertyIt != layerPropertiesIt->second.end())
			{
				isGraphLayer = (propertyIt->second == "true");
			}
		}

		for (size_t rowIndex = 0; rowIndex < aTileMapData.mLayers[layerIndex].size(); ++rowIndex)
		{
			for (size_t colIndex = 0; colIndex < aTileMapData.mLayers[layerIndex][rowIndex].size(); ++colIndex)
			{
				int tile = aTileMapData.mLayers[layerIndex][rowIndex][colIndex];
				if (tile != 0)
				{
					auto it = aTileMapData.mTileInfoMap.find(tile);
					if (it != aTileMapData.mTileInfoMap.end())
					{
						//Don't create game objects for graph layers
						if (!isGraphLayer)
						{
							createTile(aScene, it->second, aTileMapData.mLayerNames[layerIndex], layerIndex, rowIndex,
									   colIndex, aTileWidth, aTileHeight);
						}
					}
					else
					{
						throw std::runtime_error("Tile not found in tileInfoMap in LevelBuilder::createTileLayers");
					}
				}
			}
		}
	}
}

/**
 * @brief This function creates a specific tile with a the rigidbody and colliders, and is called from createTileLayers
 *
 * @param scene
 * @param tileInfo
 * @param layerName
 * @param layerIndex
 * @param rowIndex
 * @param colIndex
 */
void LevelBuilder::createTile(Scene& aScene, const TileInfo& aTileInfo, const std::string& aLayerName, int aLayerIndex,
							  int aRowIndex, int aColIndex, int aTileWidth, int aTileHeight) const
{
	EngineBravo& engine = EngineBravo::getInstance();

	SpriteDef spriteDef = {aTileInfo.mTilesetName,
						   Rect{aTileInfo.mCoordinates.first, aTileInfo.mCoordinates.second, aTileWidth, aTileHeight}, static_cast<float>(aTileWidth), static_cast<float>(aTileHeight)};

	GameObject* gameObject = new GameObject;

	Transform objectTransform;
	objectTransform.position.x = static_cast<int>(aColIndex * aTileWidth);
	objectTransform.position.y = static_cast<int>(aRowIndex * aTileHeight);
	gameObject->setTransform(objectTransform);
	gameObject->setName("Tile");

	Sprite* sprite = engine.getResourceManager().createSprite(spriteDef);
	sprite->setLayer(aLayerIndex);
	gameObject->addComponent(sprite);

	for (const auto& collider : aTileInfo.mColliders)
	{
		BoxCollider* boxCollider = new BoxCollider();
		Transform transform;
		transform.position.x = collider.x;
		transform.position.y = collider.y;
		boxCollider->setTransform(transform);
		boxCollider->setWidth(collider.mWidth + 0.1f);
		boxCollider->setHeight(collider.mHeight + 0.1f);
		boxCollider->setCollideCategory(1);
		boxCollider->setCollideWithCategory({1, 2, 3});
		gameObject->addComponent(boxCollider);
	}

	if (!aTileInfo.mColliders.empty())
	{
		RigidBody* rigidBody = new RigidBody();
		rigidBody->setTransform(objectTransform);
		rigidBody->setFriction(1.0f);
		gameObject->addComponent(rigidBody);
		gameObject->setName("Tile");
	}

	gameObject->setTag(aLayerName);
	aScene.addGameObject(gameObject);
}