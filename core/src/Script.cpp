#include "Script.h"
namespace Core
{
    void ScriptMoveEntityConstantly::run()
    {
        for (auto e : *mtargetList)
        {
            glm::vec2 loc = e->getLocation();
            e->setLocation(glm::vec2(loc.x + mdistance.x, loc.y + mdistance.y));
        }
    }

}