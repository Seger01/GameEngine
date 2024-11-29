#include "NetworkTransform.h"
#include <stdexcept>

NetworkTransform::NetworkTransform(bool aSendPositionX, bool aSendPositionY, bool aSendRotation, bool aSendScaleX,
                                   bool aSendScaleY, std::string aTag)
    : Component{aTag}, mSendPositionX(aSendPositionX), mSendPositionY(aSendPositionY), mSendRotation(aSendRotation),
      mSendScaleX(aSendScaleX), mSendScaleY(aSendScaleY) {}

std::unique_ptr<Component> NetworkTransform::clone() const { return std::make_unique<NetworkTransform>(*this); }

void NetworkTransform::setSendPositionX(bool aSendPositionX) { mSendPositionX = aSendPositionX; }

void NetworkTransform::setSendPositionY(bool aSendPositionY) { mSendPositionY = aSendPositionY; }

void NetworkTransform::setSendRotation(bool aSendRotation) { mSendRotation = aSendRotation; }

void NetworkTransform::setSendScaleX(bool aSendScaleX) { mSendScaleX = aSendScaleX; }

void NetworkTransform::setSendScaleY(bool aSendScaleY) { mSendScaleY = aSendScaleY; }

bool NetworkTransform::getSendPositionX() const { return mSendPositionX; }

bool NetworkTransform::getSendPositionY() const { return mSendPositionY; }

bool NetworkTransform::getSendRotation() const { return mSendRotation; }

bool NetworkTransform::getSendScaleX() const { return mSendScaleX; }

bool NetworkTransform::getSendScaleY() const { return mSendScaleY; }