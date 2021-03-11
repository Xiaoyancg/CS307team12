#include "Page.h"

#ifdef __TEST_CORE
#include <TestCore.h>
#endif // __TEST_CORE

namespace Core
{
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