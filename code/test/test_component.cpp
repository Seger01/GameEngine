#include "Component.h"
#include <gtest/gtest.h>
#include <memory>

TEST(ComponentTest, Initialization)
{
	Component* component = new Component();
	EXPECT_TRUE(component->isActive());
	component->setActive(false);
	EXPECT_FALSE(component->isActive());
	EXPECT_EQ(component->getTag(), "defaultComponent");
	component->setTag("newTag");
	EXPECT_EQ(component->getTag(), "newTag");

	std::unique_ptr<Component> clone = component->clone();

	delete component;
}
