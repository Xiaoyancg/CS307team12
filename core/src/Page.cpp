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
}