#pragma once

#include <string>
#include <vector> 

class GameObject
{
    public:
        GameObject();
        ~GameObject();
        //void addComponent(Component& aComponent); Object of pointer afhankelijk in Scene
        void removeComponent(int id);
        //Component& getComponent(int id);  Object of pointer afhankelijk in Scene       
        bool isActiveInWorld();
        bool isActiveSelf(); 
        void setID(int id);
        int getID();
        void setName(std::string name);
        std::string getName();
        void setTag(std::string tag);
        std::string getTag();
        void setIsActive(bool isActive);
        bool getIsActive();
    private:
        //std::vector<Component> mComponents; Object of pointer?
        int mID;
        std::string mName;
        std::string mTag;
        bool mIsActive;
        //mLayer ???
};