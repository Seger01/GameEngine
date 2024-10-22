#pragma once

class ResourceManager 
{
    public:
        friend class EngineBravo;
        void loadTextures(); //Path of ID toevoegen ?
        void loadAudio(); //Path of ID toevoegen ?
    private:
        ResourceManager();
        ~ResourceManager();
};