#include "IButtonBehaviourScript.h"
#include <gtest/gtest.h>
#include <memory>

TEST(ButtonBehaviourScriptTest, Initialization)
{
	IButtonBehaviourScript* buttonBehaviourScript = new IButtonBehaviourScript();

	EXPECT_NO_THROW(buttonBehaviourScript->onButtonPressed());
	EXPECT_NO_THROW(buttonBehaviourScript->onButtonReleased());
	EXPECT_NO_THROW(buttonBehaviourScript->onButtonHover());
	EXPECT_NO_THROW(buttonBehaviourScript->onButtonUnhover());

	std::unique_ptr<Component> clone = buttonBehaviourScript->clone();

	delete buttonBehaviourScript;
}
