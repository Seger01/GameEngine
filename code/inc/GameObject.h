#pragma once

#include <vector>
#include <memory>
#include <string>
#include <array>
#include "IComponent.h"
#include "PhysicsComponent.h"
#include "RenderComponent.h"
#include "ComponentFactory.h"

class GameObject {
public:
    GameObject();
    bool addComponent(const std::string&);
    bool removeComponent(const std::string&);
    void Update(double deltaTime);
    
    //Getters
    const std::string& getName() const;
    int getId() const;
    
    //Setters
    void setName(const std::string&);

    RenderComponent* getRenderComponent();
    PhysicsComponent* getPhysicsComponent();
private:
    std::string name;
    int id;

    std::vector<std::unique_ptr<IComponent>> components;

    static int nextId;

    ComponentFactory componentFactory;
};