#ifndef COMPONENT_H
#define COMPONENT_H

class GameObject;

class Component {
public:
    Component() {}
    virtual ~Component() {}

    void setGameObjectParent(GameObject* aParentObject) { mGameObject = aParentObject; }

    bool isActive() { return mActive; }
    void setActive(bool aState) { mActive = aState; }

protected:
    GameObject* mGameObject = nullptr;

    bool mActive = true;
};

#endif
