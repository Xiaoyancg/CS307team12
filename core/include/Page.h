#pragma once

#include <cstdio>
#include <string>
#include <vector>

#include <SDL.h>
#include <glad/glad.h>
#include <glm/core/type_mat.hpp>

#include <nlohmann/json.hpp>
#include <iterator>
#include <Entity.h>


namespace Core
{
// Page class
// Pages can be menu, settings, HUD, map, Cutscene, ...
// Each page contains its own context and render function
    class Page
    {

    public:
        Page();
        Page(std::string name);

        // Render the given page on the current context
        // Returns non-zero on error
        void setName ( std::string name );
        std::string getName ();
        void SetBackgroundColor(float r, float g, float b, float a);
        glm::vec4 GetBackgroundColor();
        std::vector<Entity*>& getEntityList ();
        virtual void render ();

        static Page* parse(nlohmann::json& root);
        nlohmann::json serialize();

        // =========================
        // CONSTRUCTOR

        // =========================
        // ATTRIBUTES OPERATION

        // =========================
        // PROPERTY OPERATION

        // =========================
        // MEMBER OPERATION
        Entity *addEntity ( Entity * );
        Entity *createEntity ( std::string );
        Entity *createEntity ( std::string, glm::vec2, glm::vec2, double, int );
        void deleteEntity (std::string);
        void deleteEntity (Entity*);
        // =========================
        // STATE OPERATION
        Entity *getCtrlEntity ();

        // TODO: should have error checking
        void setCtrlEntity ( Entity * );

        // =========================
        // UTILITY OPERATION


    protected:
        // the eneity that bind to keys of current page
        Entity *ctrlEntity;
        std::string name;
        glm::vec4 backgroundColor;
        std::vector<Entity*> entityList;
    };
}