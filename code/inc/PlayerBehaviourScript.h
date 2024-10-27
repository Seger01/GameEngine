#ifndef PLAYERBEHAVIOURSCRIPT_H
#define PLAYERBEHAVIOURSCRIPT_H

#include "Components/IBehaviourScript.h"

class PlayerBehaviourScript : public IBehaviourScript {
public:
    void onStart() override;
    void onUpdate() override;

    void setFlipX(bool aState);
    void setFlipY(bool aState);
};

#endif
