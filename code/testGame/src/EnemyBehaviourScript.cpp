#include "EnemyBehaviourScript.h"
#include "EngineBravo.h"

void EnemyBehaviourScript::onStart() {
}

void EnemyBehaviourScript::onUpdate() {
	if (isOwner())
	{ // if owner set the data to be synchronized with the network variable
		mTransform.getValue().setTransform(mGameObject->getTransform());
	}
	else
	{ // if not owner set the data to be synchronized with the network variable
		mGameObject->getTransform().position = mTransform.getValue().getTransform().position;
	}
}

void EnemyBehaviourScript::onCollide(GameObject* aGameObject) {
}

void EnemyBehaviourScript::toggleAnimaionEnabled() {
}

void EnemyBehaviourScript::setAnimationActive(std::string aAnimationTag, bool aState) {
}

void EnemyBehaviourScript::deactivateAllAnimations() {
}

void EnemyBehaviourScript::setFlipX(bool aState) {
}

void EnemyBehaviourScript::setFlipY(bool aState) {
}

