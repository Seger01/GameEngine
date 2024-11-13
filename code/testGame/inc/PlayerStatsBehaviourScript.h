#ifndef PLAYERSTATSBEHAVIOURSCRIPT_H
#define PLAYERSTATSBEHAVIOURSCRIPT_H

#include "Components/IBehaviourScript.h"
#include "Rect.h"

class PlayerStatsBehaviourScript : public IBehaviourScript {
public:
    PlayerStatsBehaviourScript();

    void onStart() override;
    void onUpdate() override;
    void onCollide(GameObject* aGameObject) override;

private:
    float mHealth;
    float mMaxHealth;
    float mHealthBarWidth;

    Rect mFullHealthBarForegroundSourceRect;
};

#endif // PLAYERSTATSBEHAVIOURSCRIPT_H
