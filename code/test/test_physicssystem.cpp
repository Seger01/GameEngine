#include "Animation.h"
#include "Camera.h"
#include "Color.h"
#include "EngineBravo.h"
#include "GameObject.h"
#include "ParticleEmitter.h"
#include "PhysicsManager.h"
#include "RenderSystem.h"
#include "Renderer.h"
#include "Scene.h"
#include "Sprite.h"
#include "SpriteDefUtil.h"
#include "Text.h"
#include "Vector2.h"
#include "Window.h"
#include <gtest/gtest.h>

class PhysicsTest : public ::testing::Test {
protected:
    // You can initialize common resources here
    PhysicsManager* mPhysicsManager;
    PhysicsEngine* mPhysicsEngine;
    RenderSystem* mRenderSystem;

    void SetUp() override {
        // Initialize the RenderSystem and necessary components
        EngineBravo& engineBravo = EngineBravo::getInstance();
        mPhysicsManager = &engineBravo.getPhysicsManager();
    }
    void TearDown() override {
        // Clean up
        EngineBravo::getInstance().getSceneManager().removeScene("Test Scene");
    }
};
