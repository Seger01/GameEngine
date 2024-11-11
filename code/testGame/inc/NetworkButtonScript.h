#ifndef NETWORKBUTTONSCRIPT_H
#define NETWORKBUTTONSCRIPT_H

#include "Components/IButtonBehaviourScript.h"

class NetworkButtonScript : public IButtonBehaviourScript {
public:
    NetworkButtonScript();
    ~NetworkButtonScript();

    void onButtonPressed() override;

    void onButtonReleased() override;

private:
    void setButtonsVisibility();

private:
    bool mRoleSelect;
};

#endif // NETWORKBUTTONSCRIPT_H