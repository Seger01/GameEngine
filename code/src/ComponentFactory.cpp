#include "ComponentFactory.h"

std::unique_ptr<IComponent> ComponentFactory::createComponent(const std::string& type) {
    if (type == "RenderComponent") {
        return std::make_unique<RenderComponent>();
    } else if (type == "PhysicsComponent") {
        return std::make_unique<PhysicsComponent>();
    } 
    return nullptr; 
}