#include "RenderComponent.h"
#include <iostream>


RenderComponent::RenderComponent() {}


void RenderComponent::Update(double deltaTime) 
{
    std::cout << "Updating Render Component" << std::endl;
}

void RenderComponent::Render() 
{
    std::cout << "Rendering" << std::endl;
}

const char* RenderComponent::GetType() const{
        return "RenderComponent";
}

const std::string& RenderComponent::getTexturePath() const {
    return texturePath;
}

double RenderComponent::getOpacity() const {
    return opacity;
}

void RenderComponent::setTexturePath(const std::string& Path) {
    texturePath = Path;
}

void RenderComponent::setOpacity(double Opacity) {
    opacity = Opacity;
}