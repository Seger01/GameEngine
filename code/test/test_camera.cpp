#include "Camera.h"
#include "Color.h"
#include "FRect.h"
#include "Time.h"
#include "Transform.h"
#include "Vector2.h"
#include <gtest/gtest.h>

// Test initialization and basic functionality
TEST(CameraTest, CameraInitialization)
{
	Camera camera;

	// Test background color
	camera.setBackgroundColor(Color(255, 255, 255, 255));
	EXPECT_EQ(camera.getBackgroundColor(), Color(255, 255, 255, 255));

	// Test transform
	camera.setTransform(Transform());
	EXPECT_EQ(camera.getTransform(), Transform());

	// Test width and height
	camera.setWidth(800);
	EXPECT_EQ(camera.getWidth(), 800);

	camera.setHeight(600);
	EXPECT_EQ(camera.getHeight(), 600);

	// Test viewport
	camera.setViewport(FRect{0.0f, 0.0f, 1.0f, 1.0f});
	EXPECT_EQ(camera.getViewport().x, 0.0f);
	EXPECT_EQ(camera.getViewport().y, 0.0f);
	EXPECT_EQ(camera.getViewport().w, 1.0f);
	EXPECT_EQ(camera.getViewport().h, 1.0f);

	// Test origin
	Vector2 cameraOrigin = camera.getOrigin();
	EXPECT_EQ(cameraOrigin.x, -400);
	EXPECT_EQ(cameraOrigin.y, -300);

	// Test debug overlay
	CameraDebugOverlay overlay;
	camera.setDebugOverlay(overlay);
	EXPECT_EQ(camera.getDebugOverlay().showFPS, overlay.showFPS);
	EXPECT_EQ(camera.getDebugOverlay().renderCameraViewport, overlay.renderCameraViewport);
	EXPECT_EQ(camera.getDebugOverlay().renderColliders, overlay.renderColliders);

	EXPECT_EQ(camera.getDebugOverlayRef().showFPS, camera.getDebugOverlay().showFPS);
	EXPECT_EQ(camera.getDebugOverlayRef().renderCameraViewport, camera.getDebugOverlay().renderCameraViewport);
	EXPECT_EQ(camera.getDebugOverlayRef().renderColliders, camera.getDebugOverlay().renderColliders);

	// Test render order
	camera.setRenderOrder(5);
	EXPECT_EQ(camera.getRenderOrder(), 5);

	// Test main camera property
	EXPECT_TRUE(camera.isMainCamera());
	camera.setMainCamera(false);
	EXPECT_FALSE(camera.isMainCamera());
}

// Test camera shake functionality
TEST(CameraTest, CameraShake)
{
	Camera camera;

	// Ensure the camera starts without shaking
	EXPECT_TRUE(camera.isMainCamera());

	// Start shaking
	camera.startShake(100.0f, 10.0f);
	EXPECT_TRUE(camera.isMainCamera());

	// Simulate updates
	Time::ticks = 50; // Halfway through the shake
	camera.update();

	// Stop shaking
	Time::ticks = 200; // Shake should end
	camera.update();
	camera.setMainCamera(false);
	EXPECT_FALSE(camera.isMainCamera());
}
