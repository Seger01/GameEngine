#include "EnemyBehaviourScript.h"

void EnemyBehaviourScript::onStart() {
    std::cout << "EnemyBehaviourScript::onStart()" << std::endl;
}

void EnemyBehaviourScript::onUpdate() {
    std::cout << "EnemyBehaviourScript::onUpdate()" << std::endl;
}

void EnemyBehaviourScript::onCollide(GameObject* aGameObject) {
    std::cout << "EnemyBehaviourScript::onCollide()" << std::endl;
}

void EnemyBehaviourScript::toggleAnimaionEnabled() {
    std::cout << "EnemyBehaviourScript::toggleAnimaionEnabled()" << std::endl;
}

void EnemyBehaviourScript::setAnimationActive(std::string aAnimationTag, bool aState) {
    std::cout << "EnemyBehaviourScript::setAnimationActive()" << std::endl;
}

void EnemyBehaviourScript::deactivateAllAnimations() {
    std::cout << "EnemyBehaviourScript::deactivateAllAnimations()" << std::endl;
}

void EnemyBehaviourScript::setFlipX(bool aState) {
    std::cout << "EnemyBehaviourScript::setFlipX()" << std::endl;
}

void EnemyBehaviourScript::setFlipY(bool aState) {
    std::cout << "EnemyBehaviourScript::setFlipY()" << std::endl;
}

