#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "Scene.h"

class ParticleSystem {
public:
    ParticleSystem();

    void update(Scene* aScene);

public:
    void addObject(GameObject& aObject);
    void removeObject(GameObject& aObject);

private:
    std::vector<std::reference_wrapper<GameObject>> mObjects;
};

#endif // PARTICLESYSTEM_H
