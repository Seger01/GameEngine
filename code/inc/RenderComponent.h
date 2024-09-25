#pragma once
#include "IComponent.h"
#include <iostream>

class RenderComponent : public IComponent {
    public:
        RenderComponent();
        void Update(double deltaTime);
        void Render();
        const char* GetType() const;

        const std::string& getTexturePath() const;
        double getOpacity() const;

        void setTexturePath(const std::string&);
        void setOpacity(double);
    private:
        std::string texturePath;
        double opacity;
};