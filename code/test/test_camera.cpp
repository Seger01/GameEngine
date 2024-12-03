#include "Configuration.h"
#include "EngineBravo.h"
#include <gtest/gtest.h>

// Test setting and getting configuration values
TEST(CameraTest, CameraInitialization)
{
	Camera camera;
	camera.setBackgroundColor(Color(255, 255, 255, 255));
	EXPECT_EQ(camera.getBackgroundColor(), Color(255, 255, 255, 255));

	camera.setTransform(Transform());

	camera.setWidth(800);
	EXPECT_EQ(camera.getWidth(), 800);

	camera.setHeight(600);
	EXPECT_EQ(camera.getHeight(), 600);

	camera.setViewport(FRect{0.0f, 0.0f, 1.0f, 1.0f});
	EXPECT_EQ(camera.getViewport().x, 0.0f);
	EXPECT_EQ(camera.getViewport().y, 0.0f);
	EXPECT_EQ(camera.getViewport().w, 1.0f);
	EXPECT_EQ(camera.getViewport().h, 1.0f);

	Vector2 cameraOrigin = camera.getOrigin();
	EXPECT_EQ(cameraOrigin.x, -400);
	EXPECT_EQ(cameraOrigin.y, -300);
}
