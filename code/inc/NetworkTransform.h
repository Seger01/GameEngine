#ifndef NETWORKTRANSFORM_H
#define NETWORKTRANSFORM_H

class NetworkTransform
{
public:
    void updateTransform();
    void syncPosition();
    void interpolate();
};

#endif // NETWORKTRANSFORM_H