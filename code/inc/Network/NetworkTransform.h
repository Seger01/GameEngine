#ifndef NETWORKTRANSFORM_H
#define NETWORKTRANSFORM_H

#include "Components/Component.h"

class NetworkTransform : public Component {
public:
    NetworkTransform(bool aSendPositionX = false, bool aSendPositionY = false, bool aSendRotation = false,
                     bool aSendScaleX = false, bool aSendScaleY = false, std::string aTag = "defaultNetworkTransform");

    std::unique_ptr<Component> clone() const override;

    void setSendPositionX(bool aSendPositionX);
    void setSendPositionY(bool aSendPositionY);
    void setSendRotation(bool aSendRotation);
    void setSendScaleX(bool aSendScaleX);
    void setSendScaleY(bool aSendScaleY);

    bool getSendPositionX() const;
    bool getSendPositionY() const;
    bool getSendRotation() const;
    bool getSendScaleX() const;
    bool getSendScaleY() const;

private:
    bool mSendPositionX;
    bool mSendPositionY;
    bool mSendRotation;
    bool mSendScaleX;
    bool mSendScaleY;
};

#endif // NETWORKTRANSFORM_H