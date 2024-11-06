#include "FSConverter.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "Window.h"
#include <SDL.h>
#include <gtest/gtest.h>

class RendererTest : public ::testing::Test {
protected:
    Window* mWindow;
    Renderer* mRenderer;

    void SetUp() override {
        mWindow = new Window(800, 600);
        mRenderer = new Renderer(*mWindow);
    }

    void TearDown() override {
        delete mRenderer;
        delete mWindow;
        mRenderer = nullptr;
        mWindow = nullptr;
    }
};

TEST_F(RendererTest, Initialization) {
    ASSERT_NO_THROW([&]() { ASSERT_NE(mRenderer->getSDLRenderer(), nullptr); }());
}

TEST_F(RendererTest, RenderTexture) {
    ASSERT_NO_THROW(([&]() {
        std::string aPngPath = "enter_the_gungeon_spritesheet.png";

        if (mRenderer == nullptr) {
            std::cout << "No renderer available" << std::endl;
            return;
        }

        SDL_Texture* sdlTexture =
            IMG_LoadTexture(mRenderer->getSDLRenderer(), FSConverter().getResourcePath(aPngPath).c_str());
        if (!sdlTexture) {
            std::cerr << "Failed to load texture from " << aPngPath << ": " << SDL_GetError() << std::endl;
            return;
        }

        int textureIDCounter = 0;
        Texture* texture = new Texture(sdlTexture, textureIDCounter);

        Rect sourceRect{0, 0, 10, 10};
        Vector2 location{100, 100};

        mRenderer->clear(Color{0, 0, 0, 255});
        mRenderer->renderTexture(*texture, sourceRect, location, 100, 100, false, false, 0.0f);

        sourceRect = {2000, 2000, -100, -100};
        mRenderer->renderTexture(*texture, sourceRect, location, -100, -100, true, true, 45.0f);

        sourceRect = {-2000, -2000, -100, -100};
        mRenderer->renderTexture(*texture, sourceRect, location, -100, -100, false, true, 45.0f);

        mRenderer->show();
    }()));
}

TEST_F(RendererTest, RenderSquare) {
    ASSERT_NO_THROW(([&]() {
        Color color{255, 0, 0, 255}; // Red color
        mRenderer->renderSquare(Vector2(200, 200), 50, 50, color, true);

        mRenderer->renderSquare(Vector2(-200000, 20000), -50, 50, color, false);
        mRenderer->renderSquare(Vector2(-200000, 20000), -50, 50, color, true);
    }()));
}

TEST_F(RendererTest, RenderText) {
    ASSERT_NO_THROW(([&]() {
        Color color{255, 255, 255, 255}; // White color
        mRenderer->renderText("Test Text", Vector2{300, 300}, color, 1.0f, 1.0f);

        mRenderer->renderText("Test Text", Vector2{-20000, 200000}, color, -1.0f, -1.0f);
    }()));
}

TEST_F(RendererTest, ClearAndShow) {
    ASSERT_NO_THROW(([&]() {
        Color clearColor{0, 0, 0, 255};
        mRenderer->clear(clearColor);
        mRenderer->show();
    }()));
}
