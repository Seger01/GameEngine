#include "GameObject.h"
#include <algorithm> 

int GameObject::nextId = 0;

GameObject::GameObject() 
    : id(nextId++) {
        setName("Unnamed Object");
    }


bool GameObject::addComponent(const std::string& type) {
    auto component = componentFactory.createComponent(type);
    if (component) {
        components.push_back(std::move(component)); // Add the created component to the list
        return true;
    }
    return false;
}


bool GameObject::removeComponent(const std::string& type) {
    auto it = std::find_if(components.begin(), components.end(),
                           [&type](const std::unique_ptr<IComponent>& component) {
                               return component->GetType() == type;
                           });

    if (it != components.end()) {
        components.erase(it);  // Remove the component from the vector
        return true;
    }
    return false;
}

// Update all components
void GameObject::Update(double deltaTime) {
    for (auto& component : components) {
        component->Update(deltaTime);
    }
}

const std::string& GameObject::getName() const {
    return name;
}

void GameObject::setName(const std::string& newName) {
    name = newName;
}

int GameObject::getId() const {
    return id;
}


RenderComponent* GameObject::getRenderComponent() {
    for (const auto& component : components) {
        if (auto renderComponent = dynamic_cast<RenderComponent*>(component.get())) {
            return renderComponent; // Return pointer to the RenderComponent
        }
    }
    return nullptr;
}

PhysicsComponent* GameObject::getPhysicsComponent() {
    for (const auto& component : components) {
        if (auto physicsComponent = dynamic_cast<PhysicsComponent*>(component.get())) {
            return physicsComponent; // Return pointer to the PhysicsComponent
        }
    }
    return nullptr;
}

