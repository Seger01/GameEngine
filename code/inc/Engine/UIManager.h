#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "UIObject.h"

class UIManager
{
    public:
        friend class EngineBravo;
        bool ClickableButtonPressed();
        bool checkIsClickableButton();
        void addClickableButton(UIObject& aButton);
        void removeClickableButton(std::string ID);
    private:
        //std::vector<UIObject&> mClickableButtons;
        UIManager();
        ~UIManager();
};