#include "Action.h"
namespace Core
{
    void ActionMoveEntityConstantly::evoke()
    {
        for (auto e : *mtargetList)
        {
            glm::vec2 loc = e->getLocation();
            e->setLocation(glm::vec2(loc.x + mdistance.x, loc.y + mdistance.y));
        }
    }

}