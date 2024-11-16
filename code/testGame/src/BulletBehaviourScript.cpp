#include "BulletBehaviourScript.h"

void BulletBehaviourScript::onStart() {
}

void BulletBehaviourScript::onUpdate() {
}

void BulletBehaviourScript::onCollide(GameObject* aGameObject) {
    std::cout << "BulletBehaviourScript collided with " << aGameObject->getName() << std::endl;
    //aGameObject->takeDamage()
    //Remove bullet
}