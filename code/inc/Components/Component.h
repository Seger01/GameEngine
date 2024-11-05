#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>

class GameObject;

class Component {
public:
    Component() {}
    virtual ~Component() {}

    void setGameObjectParent(GameObject* aParentObject) { mGameObject = aParentObject; }

    bool isActive() { return mActive; }
    void setActive(bool aState) { mActive = aState; }

    std::string getTag() { return mTag; }
    void setTag(const std::string& aTag) { mTag = aTag; }

protected:
    GameObject* mGameObject = nullptr;

    std::string mTag = "";
    bool mActive = true;
};

#endif
