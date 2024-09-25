#pragma once
#include <memory>
#include <string>
#include "IComponent.h"
#include "RenderComponent.h"
#include "PhysicsComponent.h"

class ComponentFactory {
public:
    std::unique_ptr<IComponent> createComponent(const std::string&);
};