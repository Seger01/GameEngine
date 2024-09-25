#pragma once

class IComponent {
public:
    virtual ~IComponent() = default;
    virtual void Update(double deltaTime) = 0;
    virtual const char* GetType() const = 0;
};
