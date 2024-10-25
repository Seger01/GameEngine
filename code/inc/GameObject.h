#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

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

    bool isActiveInWorld();
    bool isActiveSelf();

    void setID(int id);
    int getID();

    void setName(std::string name);
    std::string getName();

    void setTag(std::string tag);
    std::string getTag();

    void setIsActive(bool isActive);
    bool getIsActive();

    Transform getTransform();
    void setTransform(Transform aNewTransform);

    // Templated functions
    template <typename T> bool hasComponent() const {
        for (const auto& component : mComponents) {
            if (dynamic_cast<T*>(component) != nullptr) {
                return true;
            }
        }
        return false;
    }

    template <typename T> T* getComponent() const {
        for (const auto& component : mComponents) {
            if (T* casted = dynamic_cast<T*>(component)) {
                return casted;
            }
        }
        return nullptr;
    }

    // Templated addComponent function
    template <typename T, typename... Args> T* addComponent(Args&&... args) {
        // Check if the component already exists
        if (hasComponent<T>()) {
            return getComponent<T>();
        }

        // Create a new component of type T
        T* newComponent = new T(std::forward<Args>(args)...);

        newComponent->setGameObjectParent(this);

        // Add the new component to the list
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
