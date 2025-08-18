/**
 * @file IButtonBehaviourScript.h
 * @brief Defines the IButtonBehaviourScript class.
 */

#ifndef IBUTTONBEHAVIOURSCRIPT_H
#define IBUTTONBEHAVIOURSCRIPT_H

#include "Component/Component.h"

/**
 * @class IButtonBehaviourScript
 * @brief Interface for button behaviour scripts.
 */

class IButtonBehaviourScript : public Component
{
public:
	IButtonBehaviourScript(std::string aTag = "defaultButtonBehaviourScript") : Component(aTag) {}

	virtual ~IButtonBehaviourScript() {}

	virtual std::unique_ptr<Component> clone() const override
	{
		return std::make_unique<IButtonBehaviourScript>(*this);
	}

	virtual void onButtonPressed() {}

	virtual void onButtonReleased() {}

	virtual void onButtonHover() {}

	virtual void onButtonUnhover() {}
};

#endif // IBUTTONBEHAVIOURSCRIPT_H
