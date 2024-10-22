#pragma once

class IMouseListener
{
    public:
        //Clicked and Pressed methods?
        virtual ~IMouseListener() {}
        virtual void onMouseClicked(int button) = 0;
        virtual void onMouseReleased(int button) = 0;
        virtual void onMousePressed(int button) = 0;
        virtual void onMouseMoved(int x, int y) = 0;
};