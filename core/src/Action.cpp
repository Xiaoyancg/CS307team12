#include "Action.h"
namespace Core
{
    MoveEntityConstantly::MoveEntityConstantly(/* args */)
    {
    }

    MoveEntityConstantly::~MoveEntityConstantly()
    {
    }

    void MoveEntityConstantly::evoke()
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