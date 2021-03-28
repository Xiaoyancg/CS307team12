#pragma once
#include <vector>
#include <variant>
#include "Entity.h"
namespace Core
{
    // forwar declaration
    enum class SignalType;
    class Signal;
    class Logic;

    // action type
    enum class ActionType
    {
        ActionMoveEntityConstantly // move entities in constant speed
    };
    // *actions
    class ActionMoveEntityConstantly;

    //* ---------------------------- Different actions --------------------------- */

    class ActionMoveEntityConstantly
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

        ActionMoveEntityConstantly(/* args */);
        ~ActionMoveEntityConstantly();
    };
    typedef std::variant<ActionMoveEntityConstantly> ActionVariant;
    //// the action list main loop calls
    ////std::vector<ActionVariant> actionList;

    //// ---------------------------- Entry Action ---------------------------- */

    ////// the Action class for outside
    ////class Action
    ////{
    ////private:
    ////    ActionType mtype;
    ////    union
    ////    {
    ////        // name start with m for member
    ////        ActionMoveEntityConstantly mmec;
    ////    };

    ////public:
    ////    void setType(ActionType type) { mtype = type; }
    ////    ActionType getType() { return mtype; }
    ////    Action(/* args */);
    ////    ~Action();
    ////};
}