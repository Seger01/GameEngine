#pragma once
#include "Scene.h"

class Renderer
{
    public: 
        Renderer();
        ~Renderer();
        void render(const Scene& scene);
        void render(const Scene& scene, int layer);
        void clear();
        void setResolution(Point resolution);
    private:
        Point mResolution;
};