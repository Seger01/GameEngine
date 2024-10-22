#pragma once

class Button
{
    public:
        Button();
        ~Button();
        void OnClick(); //Return value?
    private:
        bool mInteractable;
};