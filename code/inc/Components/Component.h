#ifndef COMPONENT_H
#define COMPONENT_H

class GameObject;

class Component {
public:
    Component() {}
    virtual ~Component() {}

    void setGameObjectParent(GameObject* aParentObject) { mGameObject = aParentObject; }

protected:
    GameObject* mGameObject = nullptr;

    bool mActive;
};

#endif
