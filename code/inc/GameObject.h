#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <algorithm>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <vector>

#include "Component.h"
#include "Transform.h"

class GameObject {
public:
    GameObject();
    ~GameObject();

    void addComponent(Component* aComponent);
    void removeComponent(Component* component);

    bool isActiveInWorld();
    bool isActiveSelf();

    void setID(int id);
    int getID();

    void setName(const std::string& name);
    std::string getName();

    void setTag(const std::string& tag);
    std::string getTag();

    void setActive(bool isActive);
    bool isActive();

    Transform getTransform();
    void setTransform(Transform aNewTransform);

    std::vector<Component*> getComponentsWithTag(const std::string& tag) const;

    // Templated functions
    template <typename T> bool hasComponent() const {
        for (const auto& component : mComponents) {
            if (dynamic_cast<T*>(component) != nullptr) {
                return true;
            }
        }
        return false;
    }

    template <typename T> std::vector<T*> getComponents() const {
        std::vector<T*> componentsOfType;
        for (const auto& component : mComponents) {
            if (T* casted = dynamic_cast<T*>(component)) {
                componentsOfType.push_back(casted);
            }
        }
        return componentsOfType;
    }

    // Templated addComponent function
    template <typename T, typename... Args> T* addComponent(Args&&... args) {
        T* newComponent = new T(std::forward<Args>(args)...);
        newComponent->setGameObjectParent(this);

        mComponents.push_back(newComponent);

        return newComponent;
    }

private:
    std::vector<Component*> mComponents;
    Transform mTransform;

    int mID;
    std::string mName;
    std::string mTag;
    bool mIsActive;
};

#endif
