#include "NetworkTransform.h"
#include <stdexcept>

NetworkTransform::NetworkTransform()
    : mSendPositionX(false), mSendPositionY(false), mSendRotation(false), mSendScaleX(false), mSendScaleY(false) {}

NetworkTransform::NetworkTransform(bool aSendPositionX, bool aSendPositionY, bool aSendRotation, bool aSendScaleX,
                                   bool aSendScaleY)
    : mSendPositionX(aSendPositionX), mSendPositionY(aSendPositionY), mSendRotation(aSendRotation),
      mSendScaleX(aSendScaleX), mSendScaleY(aSendScaleY) {}

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