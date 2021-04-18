#include "Page.h"

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
                if (ctrlEntity == dp) {
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
                if (ctrlEntity == p) {
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

    Page *Page::parse(json &root)
    {
        Page *page = new Page;
        page->setName(root.at("PageName").get<std::string>());
        //std::vector<json> colorVec = root.at("BackgroundColor").get<std::vector<json>>();
        //page->SetBackgroundColor(
        //    colorVec[0].get<float>(),
        //    colorVec[1].get<float>(),
        //    colorVec[2].get<float>(),
        //    colorVec[3].get<float>()
        //);
        auto entityVec = root.at("entityList").get<std::vector<json>>();
        for (json entityJson : entityVec)
        {
            Entity* ent = Entity::parse(entityJson);
            ent->setParentPage(page);
            if (ent->isControlledEntity()) {
                page->setCtrlEntity(ent);
            }
            page->entityList.push_back(ent);
        }

        // Doing nothing with this as of now, just parsing
        // TODO: Have a MenuPage be created when this boolean is true
        root.at("isMenu").get<bool>();

        return page;
    }
}