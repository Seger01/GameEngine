#ifndef PLAYERPREFAB_H
#define PLAYERPREFAB_H

#include "Animation.h"
#include "EngineBravo.h"
#include "GameObject.h"
#include "NetworkTransform.h"
#include "ParticleEmitter.h"
#include "PlayerBehaviourScript.h"
#include "SpriteDefUtil.h"
#include "Transform.h"
#include <iostream>
#include <vector>

class PlayerPrefabFactory {
public:
    static GameObject* createPlayerPrefab();

private:
    static void setTransform(GameObject* gameObject);
    static void addPlayerBehaviourScript(GameObject* gameObject);
    static void setTag(GameObject* gameObject);
    static void addNetworkTransform(GameObject* gameObject);
    static void addAnimations(GameObject* gameObject);
    static void addParticleEmitter(GameObject* gameObject);
};

#endif // PLAYERPREFAB_H