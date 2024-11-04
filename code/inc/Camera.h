#pragma once
#include "Color.h"

class Camera
{
    public:
        Camera(); 
        ~Camera();
    private:
        Color mBackgroundColor;
        int mWidth;
        int mHeight;
};