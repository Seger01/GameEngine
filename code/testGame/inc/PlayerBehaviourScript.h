#ifndef PLAYERBEHAVIOURSCRIPT_H
#define PLAYERBEHAVIOURSCRIPT_H

#include "Animation.h"
#include "Components/IBehaviourScript.h"
#include "GameObject.h"

class PlayerBehaviourScript : public IBehaviourScript {
public:
    void onStart() override;
    void onUpdate() override;
    void onCollide(GameObject* aGameObject) override;

    std::unique_ptr<Component> clone() const override { return std::make_unique<PlayerBehaviourScript>(*this); }

private:
    void setFlipX(bool aState);
    void setFlipY(bool aState);

    void initEmitter();

    void toggleAnimaionEnabled();
    void setAnimationActive(std::string aAnimationTag, bool aState);
    void deactivateAllAnimations();

    void handleAnimations();
    void handleMovement();
    void hanldeCameraMovement();
};

#endif
