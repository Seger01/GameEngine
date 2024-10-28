#ifndef GAMEOBJCET_H
#define GAMEOBJCET_H

#include <string>

class GameObject
{
public:
    GameObject(std::string aName) : mName(aName) {};

    std::string getName() const { return mName; }

private:
    std::string mName;
};

#endif // GAMEOBJCET_H