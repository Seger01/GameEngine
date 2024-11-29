#ifndef PLAYERPREFAB_H
#define PLAYERPREFAB_H

class GameObject;
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
    static void addRigidBody(GameObject* gameObject);
    static void addCollider(GameObject* gameObject);
    static void addSound(GameObject* gameObject);
    static void addNetworkObject(GameObject* gameObject);
};

#endif // PLAYERPREFAB_H