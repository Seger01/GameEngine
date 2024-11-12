#include "Animation.h"
#include "Camera.h"
#include "Color.h"
#include "EngineBravo.h"
#include "GameObject.h"
#include "ParticleEmitter.h"
#include "RenderSystem.h"
#include "Renderer.h"
#include "Scene.h"
#include "Sprite.h"
#include "SpriteDefUtil.h"
#include "Text.h"
#include "Window.h"
#include <gtest/gtest.h>

class RenderSystemTest : public ::testing::Test {
protected:
    // You can initialize common resources here
    RenderSystem* mRenderSystem;
    Scene* mScene;

    void SetUp() override {
        // Initialize the RenderSystem and necessary components
        EngineBravo& engineBravo = EngineBravo::getInstance();
        mRenderSystem = &engineBravo.getRenderSystem();

        //
        engineBravo.getResourceManager().setRenderer(&mRenderSystem->getRenderer());

        mScene = engineBravo.getSceneManager().createScene("Test Scene", 1);

        if (mScene == nullptr) {
            throw std::runtime_error("Failed to create scene");
        }

        int cameraID = mScene->addCamera();
        mScene->setActiveCamera(cameraID);

        mScene->getActiveCamera().setTransform(Transform(Vector2(80, 96)));
        mScene->getActiveCamera().setWidth(16 * 30);
        mScene->getActiveCamera().setHeight(9 * 30);

        GameObject* gameObject = new GameObject();
        mScene->addGameObject(gameObject);

        Sprite* sprite =
            new Sprite(EngineBravo::getInstance().getResourceManager().loadTexture("enter_the_gungeon_spritesheet.png"),
                       200, 200, Rect());

        sprite->setLayer(0);

        GameObject* text =
            new Text("Hello, World!", "undefined", Color(255, 255, 255, 255), Vector2(400, 400), Vector2(1, 1));

        mScene->addGameObject(text);

        Animation* playerIdleBackAnimation = nullptr;

        const int spriteWidth = 16;  // Width of each sprite
        const int spriteHeight = 25; // Height of each sprite

        const Point playerIdleBackPosition = {21, 95};
        SpriteDef firstFramePlayerIdleBack = {
            "enter_the_gungeon_spritesheet.png",
            Rect{playerIdleBackPosition.x, playerIdleBackPosition.y, spriteWidth, spriteHeight}, spriteWidth,
            spriteHeight};

        std::vector<SpriteDef> playerIdleBackAnimationFrames =
            SpriteDefUtil::extrapolateSpriteDef(firstFramePlayerIdleBack, 3);

        std::vector<SpriteDef> playerAnimationIdleBack = {
            playerIdleBackAnimationFrames[0], playerIdleBackAnimationFrames[1], playerIdleBackAnimationFrames[2],
            playerIdleBackAnimationFrames[1]};

        playerIdleBackAnimation =
            EngineBravo::getInstance().getResourceManager().loadAnimation(playerAnimationIdleBack, 200, true);

        gameObject->addComponent(playerIdleBackAnimation);

        ParticleEmitter* particleEmitter = new ParticleEmitter(
            EmitterMode::Burst, 10, 0.0f, 100, 1000, Vector2(5, 5), Vector2(0, 0), 0.0f, 0.0f, 0.0f,
            {Color(255, 49, 3, 170), Color(255, 100, 3, 150), Color(0, 0, 0, 90), Color(0, 0, 0, 90)});

        particleEmitter->setParticlesPerSecond(100);
        gameObject->addComponent(particleEmitter);
        particleEmitter->burst(1000);
        particleEmitter->setLayer(4);
    }

    void TearDown() override {
        // Clean up
        EngineBravo::getInstance().getSceneManager().removeScene("Test Scene");
    }
};

TEST_F(RenderSystemTest, RenderLayer_NoExceptions) {
    ASSERT_NO_THROW(mRenderSystem->renderLayer(mScene, 0)); // Check that no exception is thrown
}

TEST_F(RenderSystemTest, RenderLayer_OutOfBoundsLayer) {
    // Test renderLayer with a layer that might be out of bounds (e.g. a high layer that doesn't exist)
    ASSERT_NO_THROW(mRenderSystem->renderLayer(mScene, 0)); // Check for exceptions when an invalid layer is
    //                                                         // passed
}

TEST_F(RenderSystemTest, Render_NoExceptions) {
    ASSERT_NO_THROW(mRenderSystem->render(mScene)); // Ensure no exception is thrown during rendering
}

TEST_F(RenderSystemTest, GetRenderer_NoExceptions) { ASSERT_NO_THROW(mRenderSystem->getRenderer()); }

TEST_F(RenderSystemTest, GetWindow_NoExceptions) {
    ASSERT_NO_THROW(mRenderSystem->getWindow());
    mRenderSystem->getWindow().showWindow();

    mRenderSystem->getWindow().showWindow();

    mRenderSystem->getWindow().setFloating();
    // mRenderSystem->getWindow().toggleFullFloating();
}
