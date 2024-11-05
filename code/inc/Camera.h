#ifndef CAMERA_H
#define CAMERA_H

#include "Color.h"
#include "GameObject.h"

class Camera : public GameObject {
public:
    Camera();
    ~Camera();

    void setBackgroundColor(Color aColor);
    Color getBackgroundColor();

    void setWidth(int aWidth);
    int getWidth();

    void setHeight(int aHeight);
    int getHeight();

private:
    Color mBackgroundColor;
    int mWidth;
    int mHeight;
};

#endif // CAMERA_H
