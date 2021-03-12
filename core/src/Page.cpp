#include "Page.h"

#ifdef __TEST_CORE
#include <TestCore.h>
#endif // __TEST_CORE

namespace Core
{
    // =========================
    // CONSTRUCTOR

    // =========================
    // ATTRIBUTES OPERATION

    // =========================
    // PROPERTY OPERATION

    // =========================
    // MEMBER OPERATION
    Entity *Page::addEntity ( Entity *e )
    {
        this->entityList.push_back ( e );
        return e;
    }
    Entity *Page::createEntity ( std::string n )
    {
        Entity *e = new Entity ( n );
        return addEntity ( e );
    }
    Entity *Page::createEntity ( std::string n, glm::vec2 l, glm::vec2 s, double r, int sid )
    {
        Entity *e = createEntity ( n );
        e->setProperty ( l, s, r, sid );
        return e;
    }
    void Page::deleteEntity ( Entity *dp )
    {
        for ( std::vector<Entity *>::iterator ptr = entityList.begin ();
              ptr != entityList.end ();
              ptr++ )
        {
            if ( *ptr == dp )
            {
                entityList.erase ( ptr );
                delete( dp );
                dp = nullptr;
                break;
            }
        }
    }

    void Page::deleteEntity ( std::string s )
    {
        for ( std::vector<Entity *>::iterator ptr = entityList.begin ();
              ptr != entityList.end ();
              ptr++ )
        {
            if ( !( *ptr )->getName ().compare ( s ) )
            {
                Entity *p = *ptr;
                entityList.erase ( ptr );
                delete( p );
                p = nullptr;
                break;
            }
        }
    }






    // =========================
    // UTILITY OPERATION


    std::string Page::GetName ()
    {
        return this->name;
    }

    void Page::SetName ( std::string newName )
    {
        this->name = std::string ( newName );
    }

    void Page::SetBackgroundColor(float r, float g, float b, float a)
    {
        this->backgroundColor = glm::vec4(r, g, b, a);
    }

    glm::vec4 Page::GetBackgroundColor()
    {
        return this->backgroundColor;
    }

    std::vector<Entity*>& Page::getEntityList() {
        return this->entityList;
    }


    void Page::render ()
    {
        for (Entity* e : this->getEntityList()) {
            e->render();
        }

#ifdef __TEST_CORE
        pageError = glGetError ();
#endif // __TEST_CORE
    }


    using json = nlohmann::json;

    Page* Page::parse(json& root) {
        Page* page = new Page;
        page->SetName(root.at("Name").get<std::string>());
        std::vector<json> colorVec = root.at("BackgroundColor").get<std::vector<json>>();
        page->SetBackgroundColor(
            colorVec[0].get<float>(),
            colorVec[1].get<float>(),
            colorVec[2].get<float>(),
            colorVec[3].get<float>()
        );
        auto entityVec = root.at("EntityList").get<std::vector<json>>();
        for (json entityJson : entityVec) {
            page->entityList.push_back(Entity::parse(entityJson));
        }

        return page;
    }
}