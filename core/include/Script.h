#pragma once
#include <vector>
#include <variant>
#include "Entity.h"
namespace Core
{
    // forwar declaration
    enum class SignalType;
    class Logic;

    // mscript type
    enum class ScriptType
    {
        ScriptMoveEntityConstantly // move entities in constant speed
    };
    // *Script
    class ScriptMoveEntityConstantly;

    //* ------------------ DIFFERENT  SCRIPT ----------------- *//

    class ScriptBase
    {
    private:
        ScriptType mtype;

    public:
        // every script has to have run
        virtual void run() = 0;
        ScriptType getType() { return this->mtype; }
        void setType(ScriptType type) { this->mtype = type; }

        ScriptBase(ScriptType type) : mtype(type) {}
        ~ScriptBase() {}
    };

    class ScriptMoveEntityConstantly : ScriptBase
    {
    private:
        std::vector<Entity *> *mtargetList;
        //MoveType mmoveT;
        glm::vec2 mdistance;

    public:
        void setTargetList(std::vector<Entity *> *targetList)
        {
            mtargetList = targetList;
        }
        void addTarget(Entity *e) { mtargetList->push_back(e); }
        std::vector<Entity *> *getTargetList() { return mtargetList; }
        void setDistance(glm::vec2 &distance) { mdistance = distance; }
        glm::vec2 getDistance() { return mdistance; }

        void run();

        ScriptMoveEntityConstantly(ScriptType type) : ScriptBase(type) {}
        ~ScriptMoveEntityConstantly() {}
    };

    // most of the time should use pointer of this type to speed up and save
    // memory
    typedef std::variant<
        ScriptMoveEntityConstantly>
        ScriptVariant;

    //// the mscript list main loop calls
    ////std::vector<ScriptVariant> actionList;

    //// ---------------------------- Entry Action ---------------------------- */

    ////// the Action class for outside
    ////class Action
    ////{
    ////private:
    ////    ScriptType mtype;
    ////    union
    ////    {
    ////        // name start with m for member
    ////        ScriptMoveEntityConstantly mmec;
    ////    };

    ////public:
    ////    void setType(ScriptType type) { mtype = type; }
    ////    ScriptType getType() { return mtype; }
    ////    Action(/* args */);
    ////    ~Action();
    ////};
}