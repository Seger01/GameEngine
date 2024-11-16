#ifndef BULLETPREFAB_H
#define BULLETPREFAB_H

#include "Animation.h"
#include "EngineBravo.h"
#include "GameObject.h"
#include "RigidBody.h"
#include "SpriteDefUtil.h"
#include "Transform.h"
#include "Rect.h"
#include "FSConverter.h"
#include <iostream>
#include <vector>

class BulletPrefabFactory {
public:
    BulletPrefabFactory();
    GameObject* createBulletPrefab(GameObject& shooter);

private:
    void setTransform(GameObject* gameObject, Transform shooterTransform);
    void addSprite(GameObject* gameObject);
    void addRigidBody(GameObject* gameObject);
    void addCollider(GameObject* gameObject);

private:
    SpriteDef mBulletSpriteDef;
    float mBulletWidth;
    float mBulletHeight;
    Point mBulletSpritePosition;
};

#endif // BULLETPREFAB_H