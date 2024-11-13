#ifndef NETWORKBUTTONSCRIPT_H
#define NETWORKBUTTONSCRIPT_H

#include "Components/IButtonBehaviourScript.h"

#include "Text.h"

class NetworkButtonScript : public IButtonBehaviourScript {
public:
    NetworkButtonScript();
    ~NetworkButtonScript();

    std::unique_ptr<Component> clone() const override { return std::make_unique<NetworkButtonScript>(*this); }

    void onButtonPressed() override;

    void onButtonReleased() override;

private:
    void setButtonsVisibility();
};

class ConnectButtonScript : public IButtonBehaviourScript {
public:
    ConnectButtonScript(Text* aTextObject);
    ~ConnectButtonScript();

    std::unique_ptr<Component> clone() const override { return std::make_unique<ConnectButtonScript>(*this); }

    void onButtonPressed() override;

    void onButtonReleased() override;

private:
    Text* mTextObject{nullptr};
};

#endif // NETWORKBUTTONSCRIPT_H