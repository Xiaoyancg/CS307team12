#include "Script.h"
namespace Core
{
    void ScriptMoveEntityConstantly::run()
    {
        // not current pages
        if (mpageManager_ptr->getCurrPages()->find(mtargetPage) ==
            mpageManager_ptr->getCurrPages()->end())
        {
            return;
        }
        // it's target entity name, because we always use name as key, the name
        // is silent
        for (auto targetEntity : mtargetList)
        {
            Page *page_ptr = mpageManager_ptr->getCurrPages()->at(mtargetPage);
            // FIXME: use unordered_map for entities in page
            // here we assume the entity list only conatains one entity
            for (auto entity : page_ptr->getEntityList())
            {
                if (entity->getName() == targetEntity)
                {
                    glm::vec2 loc = entity->getLocation();
                    entity->setLocation(glm::vec2(loc.x + mmovement.x,
                                                  loc.y + mmovement.y));
                }
            }
        }
    }
}
}