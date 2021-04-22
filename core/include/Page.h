#pragma once
#pragma warning(disable : 5033)
#include <cstdio>
#include <string>
#include <vector>

#include <SDL.h>
#include <glad/glad.h>
#include <nlohmann/json.hpp>
#include <iterator>
#include "Entity.h"

namespace Core
{
    class Game;
    // Page class
    // Pages can be menu, settings, HUD, map, Cutscene, ...
    // Each page contains its own context and render function
    class Page
    {

    public:
        Page();
        Page(std::string name);
        explicit Page(const Page &other);
        ~Page();

        void setName(std::string name);
        std::string getName();
        void setID(int id) { _id = id; }
        int getID() { return _id; }

        void SetBackgroundColor(float r, float g, float b, float a);
        glm::vec4 GetBackgroundColor();
        std::vector<Entity *> &getEntityList();
        virtual void render();

        static Page* fromJSON(nlohmann::json &root);

        virtual void parse(nlohmann::json& root);
        virtual nlohmann::json serialize();

        Entity *addEntity(Entity *);
        Entity *createEntity(std::string);
        Entity *createEntity(std::string, glm::vec2, glm::vec2, double, int);
        /// \brief depreciate
        ///
        void deleteEntity(std::string);
        void deleteEntity(Entity *);

        Entity* getEntityWithID(int);

        Game* getGame() {
            return mGame;
        }

        void setGame(Game* game) {
            mGame = game;
        }
        
        void setInScriptId(int);
        int getInScriptId();
        void setOutScriptId(int);
        int getOutScriptId();

    protected:
        // the entity that bind to keys of current page

        int _id;

        std::string name;
        glm::vec4 backgroundColor;
        std::vector<Entity *> entityList;

        Game* mGame = nullptr;
        int mInScriptId;
        int mOutScriptId;
    };
}