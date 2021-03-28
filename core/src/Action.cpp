#include "Action.h"
namespace Core
{
    ActionMoveEntityConstantly::ActionMoveEntityConstantly(/* args */)
    {
    }

    ActionMoveEntityConstantly::~ActionMoveEntityConstantly()
    {
    }

    void ActionMoveEntityConstantly::evoke()
    {
        //if (mmoveT == MoveType::constant)
        //{
        for (auto e : *mtargetList)
        {
            glm::vec2 loc = e->getLocation();
            e->setLocation(glm::vec2(loc.x + mdistance.x, loc.y + mdistance.y));
        }
        //}
    }

    Action::Action(/* args */)
    {
    }

    Action::~Action()
    {
    }

}