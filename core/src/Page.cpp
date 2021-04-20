#include "Page.h"
#include "MapPage.h"
#include "MenuPage.h"

#ifdef __TEST_CORE
#include <TestCore.h>
#endif // __TEST_CORE

namespace Core
{
    // =========================
    // CONSTRUCTOR
    Page::Page() : Page("empty")
    {}
    Page::Page(std::string name) : name(name)
    {}
    Page::Page(const Page& other) :
        name(other.name),
        backgroundColor(other.backgroundColor),
        entityList(other.entityList.size()),
        mGame(other.mGame)
    {
        // TODO: restore ctrl entity and _id
        for (int i = 0; i < other.entityList.size(); i++) {
            entityList[i] = new Entity(*other.entityList[i]);
            entityList[i]->setParentPage(this);
            if (entityList[i]->isControlledEntity()) {
                ctrlEntity = entityList[i];
            }
        }
    }
    Page::~Page() {
        for (auto entity : entityList) {
            delete entity;
        }
    }

    // =========================
    // ATTRIBUTES OPERATION

    // =========================
    // PROPERTY OPERATION

    // =========================
    // MEMBER OPERATION
    Entity *Page::addEntity(Entity *e)
    {
        this->entityList.push_back(e);
        return e;
    }
    Entity *Page::createEntity(std::string n)
    {
        Entity *e = new Entity(n);
        // set id to -100 (default)
        e->setSpriteID(-100);
        return addEntity(e);
    }
    Entity *Page::createEntity(std::string n, glm::vec2 l, glm::vec2 s, double r, int sid)
    {
        Entity *e = createEntity(n);
        e->setProperty(l, s, r, sid);
        return e;
    }
    void Page::deleteEntity(Entity *dp)
    {
        for (std::vector<Entity *>::iterator ptr = entityList.begin();
             ptr != entityList.end();
             ptr++)
        {
            if (*ptr == dp)
            {
                entityList.erase(ptr);
                if (ctrlEntity == dp)
                {
                    ctrlEntity = nullptr;
                }
                delete (dp);
                dp = nullptr;
                break;
            }
        }
    }

    void Page::deleteEntity(std::string s)
    {
        for (std::vector<Entity *>::iterator ptr = entityList.begin();
             ptr != entityList.end();
             ptr++)
        {
            if (!(*ptr)->getName().compare(s))
            {
                Entity *p = *ptr;
                if (ctrlEntity == p)
                {
                    ctrlEntity = nullptr;
                }
                entityList.erase(ptr);
                delete (p);
                p = nullptr;
                break;
            }
        }
    }

    // =========================
    // STATE OPERATION
    // Idea is that only game has the curr(current keywoard)
    Entity *Page::getCtrlEntity()
    {
        return this->ctrlEntity;
    }

    // TODO: should have error checking
    void Page::setCtrlEntity(Entity *e)
    {
        this->ctrlEntity = e;
    }

    // =========================
    // UTILITY OPERATION

    std::string Page::getName()
    {
        return this->name;
    }

    void Page::setName(std::string newName)
    {
        this->name = std::string(newName);
    }

    void Page::SetBackgroundColor(float r, float g, float b, float a)
    {
        this->backgroundColor = glm::vec4(r, g, b, a);
    }

    glm::vec4 Page::GetBackgroundColor()
    {
        return this->backgroundColor;
    }

    std::vector<Entity *> &Page::getEntityList()
    {
        return this->entityList;
    }

    void Page::update(float dt) {
        for (auto entity : entityList) {
            entity->update(dt);
        }
    }

    void Page::render()
    {
        for (Entity *e : this->getEntityList())
        {
            e->render();
        }

#ifdef __TEST_CORE
        pageError = glGetError();
#endif // __TEST_CORE
    }

    using json = nlohmann::json;

    Page *Page::fromJSON(json &root)
    {
        Page* page;
        if (root.contains("type")) {
            std::string pageType = root.at("type").get<std::string>();
            if (pageType == "menu") {
                page = new MenuPage;
            } else if (pageType == "map") {
                page = new MapPage;
            } else {
                page = new Page;
            }
        } else {
            page = new Page;
        }
        
        page->parse(root);
        return page;
    }

    void Page::parse(json& root) {
        setName(root.at("PageName").get<std::string>());
        if (root.contains("BackgroundColor")) {
            std::vector<json> colorVec = root.at("BackgroundColor").get<std::vector<json>>();
            SetBackgroundColor(
                colorVec[0].get<float>(),
                colorVec[1].get<float>(),
                colorVec[2].get<float>(),
                colorVec[3].get<float>()
            );    
        }
        
        auto entityVec = root.at("entityList").get<std::vector<json>>();
        for (json entityJson : entityVec)
        {
            Entity* ent = Entity::fromJSON(entityJson);
            ent->setParentPage(this);
            if (ent->isControlledEntity()) {
                setCtrlEntity(ent);
            }
            entityList.push_back(ent);
        }
    }

    json Page::serialize() {
        json root;
        root["PageName"] = name;
        std::vector<json> entityVector;
        for (Entity *e : entityList)
        {
            entityVector.push_back(e->serialize());
        }
        root["entityList"] = entityVector;
        return root;
    }
}