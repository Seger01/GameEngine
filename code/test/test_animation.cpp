#include "Animation.h"
#include "EngineBravo.h"
#include "GameObject.h"
#include "Point.h"
#include "Sprite.h"
#include "SpriteDef.h"
#include "SpriteDefUtil.h"
#include "Time.h"
#include "gtest/gtest.h"

const int spriteWidth = 16;	 // Width of each sprite
const int spriteHeight = 25; // Height of each sprite

const Point playerIdleBackPosition = {21, 95};
SpriteDef firstFramePlayerIdleBack = {
	"enter_the_gungeon_spritesheet.png",
	Rect{playerIdleBackPosition.x, playerIdleBackPosition.y, spriteWidth, spriteHeight}, spriteWidth, spriteHeight};

class AnimationTest : public ::testing::Test
{
protected:
	Animation* mTestAnimation;
	int mAmountOfFrames = 4;

	void SetUp() override
	{
		EngineBravo::getInstance().initialize();
		{
			std::vector<SpriteDef> playerIdleBackAnimationFrames =
				SpriteDefUtil::extrapolateSpriteDef(firstFramePlayerIdleBack, 3);

			std::vector<SpriteDef> playerAnimationIdleBack = {
				playerIdleBackAnimationFrames[0], playerIdleBackAnimationFrames[1], playerIdleBackAnimationFrames[2],
				playerIdleBackAnimationFrames[1]};

			mTestAnimation =
				EngineBravo::getInstance().getResourceManager().loadAnimation(playerAnimationIdleBack, 200, true);
		}
	}

	void TearDown() override {}
};

TEST_F(AnimationTest, ConstructorInitializesCorrectly)
{
	Animation animation(*mTestAnimation);
	EXPECT_EQ(animation.getTimeBetweenFrames(), 200);
	EXPECT_TRUE(animation.isLooping());
	EXPECT_EQ(animation.getFrameCount(), mAmountOfFrames);
	EXPECT_EQ(animation.getTag(), "defaultAnimation");

	animation.setIsLooping(false);
	EXPECT_FALSE(animation.isLooping());
}

TEST_F(AnimationTest, CopyConstructorCreatesDeepCopy)
{
	Animation animation1(*mTestAnimation);
	Animation animation2 = animation1;

	EXPECT_EQ(animation2.getTimeBetweenFrames(), animation1.getTimeBetweenFrames());
	EXPECT_EQ(animation2.isLooping(), animation1.isLooping());
	EXPECT_EQ(animation2.getFrameCount(), animation1.getFrameCount());
	EXPECT_NE(&animation2.getFrame(0), &animation1.getFrame(0));
}

TEST_F(AnimationTest, MoveConstructorTransfersOwnership)
{
	Animation animation1(*mTestAnimation);
	Animation animation2 = std::move(animation1);

	EXPECT_EQ(animation2.getTimeBetweenFrames(), 200);
	EXPECT_TRUE(animation2.isLooping());
	EXPECT_EQ(animation2.getFrameCount(), mAmountOfFrames);
	EXPECT_EQ(animation1.getFrameCount(), 0); // animation1 should now be empty
}

TEST_F(AnimationTest, MoveAssignmentConstructor)
{
	Animation animation1(*mTestAnimation);
	Animation animation2(*mTestAnimation);
	animation2 = animation1;
}

TEST_F(AnimationTest, CloneCreatesIdenticalCopy)
{
	Animation animation(*mTestAnimation);
	std::unique_ptr<Component> clonedAnimation = animation.clone();

	EXPECT_EQ(static_cast<Animation*>(clonedAnimation.get())->getTimeBetweenFrames(), animation.getTimeBetweenFrames());
	EXPECT_EQ(static_cast<Animation*>(clonedAnimation.get())->isLooping(), animation.isLooping());
	EXPECT_EQ(static_cast<Animation*>(clonedAnimation.get())->getFrameCount(), animation.getFrameCount());
	EXPECT_NE(&static_cast<Animation*>(clonedAnimation.get())->getFrame(0), &animation.getFrame(0));
}

TEST_F(AnimationTest, SetFlipXAndFlipY)
{
	Animation animation(*mTestAnimation);

	animation.setFlipX(true);
	animation.setFlipY(true);

	EXPECT_TRUE(animation.getFlipX());
	EXPECT_TRUE(animation.getFlipY());

	const Sprite& frame = animation.getCurrentFrame();
	EXPECT_TRUE(frame.getFlipX());
	EXPECT_TRUE(frame.getFlipY());
}

TEST_F(AnimationTest, SetAndGetColorFilter)
{
	Animation animation(*mTestAnimation);

	Color newColor(128, 64, 64, 255);
	animation.setColorFilter(newColor);

	Color color = animation.getColorFilter();
	EXPECT_EQ(color.r, 128);
	EXPECT_EQ(color.g, 64);
	EXPECT_EQ(color.b, 64);
	EXPECT_EQ(color.a, 255);
}

TEST_F(AnimationTest, GetTransformAddsParentTransform)
{
	GameObject gameObject;
	Transform parentTransform({10, 20});
	gameObject.setTransform(parentTransform);

	Animation animation(*mTestAnimation);
	animation.setGameObjectParent(&gameObject);

	Transform localTransform({5, 5});
	animation.setTransform(localTransform);

	Transform resultTransform = animation.getTransform();
	EXPECT_EQ(resultTransform.position.x, 15);
	EXPECT_EQ(resultTransform.position.y, 25);
}

TEST_F(AnimationTest, SetTimeBetweenFrames)
{
	Animation animation(*mTestAnimation);
	animation.setTimeBetweenFrames(200);

	EXPECT_EQ(animation.getTimeBetweenFrames(), 200);
}

TEST_F(AnimationTest, SetAndGetLayer)
{
	Animation animation(*mTestAnimation);

	animation.setLayer(3);
	EXPECT_EQ(animation.getLayer(), 3);
}
