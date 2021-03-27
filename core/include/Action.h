#include <vector>

#include "Entity.h"
#include "Logic.h"
#include "Game.h"
namespace Core
{

    //enum class MoveType
    //{
    //    constant
    //    //,velocity
    //};

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

    class Action : MoveEntityConstantly
    {
    private:
        ActionType mtype;

    public:
        void setType(ActionType type) { mtype = type; }
        ActionType getType() { return mtype; }
        Action(/* args */);
        ~Action();
    };

}