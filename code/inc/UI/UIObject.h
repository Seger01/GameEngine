#pragma once
#include "Button.h"
#include "Text.h"
#include "GameObject.h"

class UIObject : public GameObject
{
    public:
        UIObject();
        ~UIObject();
        //add UIObject functions and remove UIObject functions?
    private:
        int mWidth;
        int mHeight;
};