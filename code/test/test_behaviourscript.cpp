#include "IBehaviourScript.h"
#include <gtest/gtest.h>
#include <memory>

TEST(BehaviourScriptTest, Initialization)
{
	IBehaviourScript* behaviourScript = new IBehaviourScript();
	EXPECT_NO_THROW(behaviourScript->onStart());
	EXPECT_NO_THROW(behaviourScript->onUpdate());
	EXPECT_NO_THROW(behaviourScript->onCollide(nullptr));
	EXPECT_FALSE(behaviourScript->hasScriptStarted());
	behaviourScript->setScriptStarted(true);
	EXPECT_TRUE(behaviourScript->hasScriptStarted());

	std::unique_ptr<Component> clone = behaviourScript->clone();

	delete behaviourScript;
}
