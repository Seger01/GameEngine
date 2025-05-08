/**
 * @file NetworkTransform.cpp
 * @brief Implementation of the NetworkTransform class.
 */

#include "Network/NetworkTransform.h"

/**
 * @brief Constructs a new NetworkTransform object.
 *
 * @param aSendPositionX Whether to send the X position.
 * @param aSendPositionY Whether to send the Y position.
 * @param aSendRotation Whether to send the rotation.
 * @param aSendScaleX Whether to send the X scale.
 * @param aSendScaleY Whether to send the Y scale.
 * @param aTag The tag for the component.
 */
NetworkTransform::NetworkTransform(bool aSendPositionX, bool aSendPositionY, bool aSendRotation, bool aSendScaleX,
								   bool aSendScaleY, const std::string& aTag)
	: Component{aTag}, mSendPositionX(aSendPositionX), mSendPositionY(aSendPositionY), mSendRotation(aSendRotation),
	  mSendScaleX(aSendScaleX), mSendScaleY(aSendScaleY)
{
}

/**
 * @brief Clones the NetworkTransform object.
 *
 * @return std::unique_ptr<Component> A unique pointer to the cloned object.
 */
std::unique_ptr<Component> NetworkTransform::clone() const { return std::make_unique<NetworkTransform>(*this); }

/**
 * @brief Sets whether to send the X position.
 *
 * @param aSendPositionX Whether to send the X position.
 */
void NetworkTransform::setSendPositionX(bool aSendPositionX) { mSendPositionX = aSendPositionX; }

/**
 * @brief Sets whether to send the Y position.
 *
 * @param aSendPositionY Whether to send the Y position.
 */
void NetworkTransform::setSendPositionY(bool aSendPositionY) { mSendPositionY = aSendPositionY; }

/**
 * @brief Sets whether to send the rotation.
 *
 * @param aSendRotation Whether to send the rotation.
 */
void NetworkTransform::setSendRotation(bool aSendRotation) { mSendRotation = aSendRotation; }

/**
 * @brief Sets whether to send the X scale.
 *
 * @param aSendScaleX Whether to send the X scale.
 */
void NetworkTransform::setSendScaleX(bool aSendScaleX) { mSendScaleX = aSendScaleX; }

/**
 * @brief Sets whether to send the Y scale.
 *
 * @param aSendScaleY Whether to send the Y scale.
 */
void NetworkTransform::setSendScaleY(bool aSendScaleY) { mSendScaleY = aSendScaleY; }

/**
 * @brief Gets whether the X position is sent.
 *
 * @return true if the X position is sent, false otherwise.
 */
bool NetworkTransform::getSendPositionX() const { return mSendPositionX; }

/**
 * @brief Gets whether the Y position is sent.
 *
 * @return true if the Y position is sent, false otherwise.
 */
bool NetworkTransform::getSendPositionY() const { return mSendPositionY; }

/**
 * @brief Gets whether the rotation is sent.
 *
 * @return true if the rotation is sent, false otherwise.
 */
bool NetworkTransform::getSendRotation() const { return mSendRotation; }

/**
 * @brief Gets whether the X scale is sent.
 *
 * @return true if the X scale is sent, false otherwise.
 */
bool NetworkTransform::getSendScaleX() const { return mSendScaleX; }

/**
 * @brief Gets whether the Y scale is sent.
 *
 * @return true if the Y scale is sent, false otherwise.
 */
bool NetworkTransform::getSendScaleY() const { return mSendScaleY; }
