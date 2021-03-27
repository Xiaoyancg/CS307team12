#pragma once
#include <vector>
#include "Entity.h"
#include "Response.h"
namespace Core
{
    // forwar declaration
    enum class SignalType;
    class Signal;
    class Response;

    // action type
    enum class ActionType
    {
        MoveEntityConstantly // move entities in constant speed
    };
    // *actions
    class MoveEntityConstantly;

    //* ---------------------------- Different actions --------------------------- */

    class MoveEntityConstantly
    {
    private:
        std::vector<Entity *> *mtargetList;
        //MoveType mmoveT;
        glm::vec2 mdistance;

    public:
        void setList(std::vector<Entity *> *targetList) { mtargetList = targetList; }
        void addEntity(Entity *e) { mtargetList->push_back(e); }
        std::vector<Entity *> *getList() { return mtargetList; }
        //void setMoveType(MoveType moveT) { mmoveT = moveT; }
        //MoveType getMoveType() { return mmoveT; }
        void setDistance(glm::vec2 &distance) { mdistance = distance; }
        glm::vec2 getDistance() { return mdistance; }
        void evoke();

        MoveEntityConstantly(/* args */);
        ~MoveEntityConstantly();
    };

    //* ---------------------------- Entry Action ---------------------------- */

    // the Action class for outside
    class Action
    {
    private:
        ActionType mtype;
        union
        {
            // name start with m for member
            MoveEntityConstantly mmec;
        };

    public:
        void setType(ActionType type) { mtype = type; }
        ActionType getType() { return mtype; }
        Action(/* args */);
        ~Action();
    };
}