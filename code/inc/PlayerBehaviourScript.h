#ifndef PLAYERBEHAVIOURSCRIPT_H
#define PLAYERBEHAVIOURSCRIPT_H

#include "Animation.h"
#include "Components/IBehaviourScript.h"

class PlayerBehaviourScript : public IBehaviourScript {
public:
    void onStart() override;
    void onUpdate() override;

private:
    void setFlipX(bool aState);
    void setFlipY(bool aState);

    void initEmitter();

    void toggleAnimaionEnabled();
    void setAnimationActive(std::string aAnimationTag, bool aState);
    void deactivateAllAnimations();
    void handleAnimations();

    void handleMovement();
};

#endif
