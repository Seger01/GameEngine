/**
 * @file IBehaviourScript.h
 * @brief IBehaviourScript class header file.
 */

#ifndef IBEHAVIOURSCRIPT_H
#define IBEHAVIOURSCRIPT_H

#include "Component.h"
#include "GameObject.h"

/**
 * @class IBehaviourScript
 * @brief Interface for behaviour scripts.
 */
class IBehaviourScript : public Component
{
public:
	IBehaviourScript(std::string aTag = "defaultBehaviourScript") : Component(aTag) {}

	virtual ~IBehaviourScript() = default;

	virtual void onStart() {}

	virtual void onUpdate() {}

	virtual void onCollide(GameObject* aGameObject) {}

	virtual std::unique_ptr<Component> clone() const override = 0;

	bool hasScriptStarted() { return hasStarted; }

	void setScriptStarted(bool aState) { hasStarted = aState; }

private:
	bool hasStarted = false;
};

#endif
