/**
 * @file NetworkTransform.h
 * @brief Header file for the NetworkTransform component.
 */

#ifndef NETWORKTRANSFORM_H
#define NETWORKTRANSFORM_H

#include "Components/Component.h"

/**
 * @class NetworkTransform
 * @brief A component that handles network synchronization of transform properties.
 */
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
    bool mSendPositionX; /**< Flag to send the X position over the network. */
    bool mSendPositionY; /**< Flag to send the Y position over the network. */
    bool mSendRotation;  /**< Flag to send the rotation over the network. */
    bool mSendScaleX;    /**< Flag to send the X scale over the network. */
    bool mSendScaleY;    /**< Flag to send the Y scale over the network. */
};

#endif // NETWORKTRANSFORM_H