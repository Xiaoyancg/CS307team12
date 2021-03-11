#pragma once

#include <cstdio>
#include <string>
#include <vector>

#include <SDL.h>
#include <glad/glad.h>
#include <glm/core/type_mat.hpp>

#include <Entity.h>


namespace Core
{
// Page class
// Pages can be menu, settings, HUD, map, Cutscene, ...
// Each page contains its own context and render function
    class Page
    {

    public:
        Page() {};
        Page(std::string name) : name(name) {};

        // Render the given page on the current context
        // Returns non-zero on error
        void SetName ( std::string name );
        std::string GetName ();
        void SetBackgroundColor(float r, float g, float b, float a);
        glm::vec4 GetBackgroundColor();
        std::vector<Entity*>& getEntityList ();
        virtual void render ();

    protected:
        std::string name;
        glm::vec4 backgroundColor;
        std::vector<Entity*> entityList;
    };
}