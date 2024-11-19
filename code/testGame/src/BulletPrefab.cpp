#include "BulletPrefab.h"

BulletPrefabFactory::BulletPrefabFactory() {
    mBulletWidth = 12;
    mBulletHeight = 12;
    mBulletSpritePosition = {137, 402};

    FSConverter fsConverter;
    std::string bulletSpritePath = fsConverter.getResourcePath("enterthegungeon_bullets.png");
    mBulletSpriteDef = {
        bulletSpritePath,
        Rect{mBulletSpritePosition.x, mBulletSpritePosition.y, mBulletWidth, mBulletHeight}, mBulletWidth, mBulletHeight};
}

GameObject* BulletPrefabFactory::createBulletPrefab(GameObject& shooter) {
    GameObject* bulletPrefab = new GameObject;
    bulletPrefab->setName("Bullet");
    bulletPrefab->addComponent<BulletBehaviourScript>();
    setTransform(bulletPrefab, shooter.getTransform());
    addSprite(bulletPrefab);
    addRigidBody(bulletPrefab);
    addCollider(bulletPrefab);

    return bulletPrefab;
}

void BulletPrefabFactory::setTransform(GameObject* gameObject, Transform shooterTransform) {
    Transform objectTransform;
    objectTransform.position.x = shooterTransform.position.x;
    objectTransform.position.y = shooterTransform.position.y;
    gameObject->setTransform(objectTransform);
}

void BulletPrefabFactory::addSprite(GameObject* gameObject) {
    Sprite* bulletSprite = EngineBravo::getInstance().getResourceManager().createSprite(mBulletSpriteDef);
    bulletSprite->setLayer(3);
    gameObject->addComponent(bulletSprite);
}

void BulletPrefabFactory::addRigidBody(GameObject* gameObject) {
    RigidBody* rigidBody = new RigidBody();

    rigidBody->setCanRotate(true);
    rigidBody->setHasGravity(false);
    rigidBody->setIsMoveableByForce(true);
    rigidBody->setDensity(1.0f);
    rigidBody->setFriction(0.0f);
    rigidBody->setRestitution(0.0f);
    rigidBody->setGravityScale(0.0f);
    rigidBody->setMass(0.0f);
    rigidBody->setLinearDamping(0.0f);
    rigidBody->setAngularDamping(0.0f);

    gameObject->addComponent(rigidBody);
}

void BulletPrefabFactory::addCollider(GameObject* gameObject) {
    BoxCollider* boxCollider = new BoxCollider();
    boxCollider->setWidth(mBulletWidth);
    boxCollider->setHeight(mBulletHeight);
    boxCollider->setTrigger(true);
    gameObject->addComponent(boxCollider);
}