#ifndef BUTTON_H
#define BUTTON_H

class Button {
public:
    Button();
    ~Button();
    void OnClick(); // Return value?
private:
    bool mInteractable;
};

#endif // BUTTON_H
