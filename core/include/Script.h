#pragma once
#include <vector>
#include <variant>
#include "Game.h"
namespace Core
{

    // anchor - script type enum class
    enum class ScriptType
    {
        ScriptMoveEntityConstantly // move entities in constant speed
    };

    /// anchor - Script Bass class
    /// \brief script can change everything in the game
    /// \note when construct, pass the reference of current game state. Script
    /// knows the state and renderer knows the resources.
    class ScriptBase
    {
    private:
        ScriptType mtype;
        /// \brief current game state, passed by constructor
        //struct GameState mstate_ref;

    public:
        // type
        ScriptType getType() { return this->mtype; }
        void setType(ScriptType type) { this->mtype = type; }

        // every script has to have run
        virtual void run() = 0;

        /// \brief Construct a new Script Base object
        /// \param type
        /// \param state_ref
        ScriptBase(ScriptType type)
            : mtype(type) {}
        ~ScriptBase() {}
    };

    //* ------------------ DIFFERENT  SCRIPT ----------------- *//
    // anchor - move entity constantly script
    /// \brief move a list of entities by a vec2 once per run
    class ScriptMoveEntityConstantly : ScriptBase
    {
    private:
        /// \brief target list
        /// \note vector::push_back copy value, so have to use pointer
        std::vector<Entity *> mtargetList;

        /// \brief the constant movement
        glm::vec2 mmovement;

    public:
        // target list
        void setTargetList(std::vector<Entity *> targetList)
        {
            mtargetList = targetList;
        }
        std::vector<Entity *> getTargetList() { return mtargetList; }
        // target
        void addTarget(Entity *entity_ptr) { mtargetList.push_back(entity_ptr); }
        // movement
        void setDistance(glm::vec2 &distance) { mmovement = distance; }
        glm::vec2 getDistance() { return mmovement; }

        /// \brief move all entities in the targget list use movement
        void run();

        /// \brief Construct a new Script Move Entity Constantly object
        /// \param state_ref game state
        ScriptMoveEntityConstantly(std::vector<Entity *> targetList,
                                   glm::vec2 movement)
            : ScriptBase(ScriptType::ScriptMoveEntityConstantly),
              mtargetList(targetList),
              mmovement(movement) {}
        ~ScriptMoveEntityConstantly() {}
    };

    /// anchor - Script Variant
    /// \brief contains all script classes
    typedef std::variant<
        ScriptMoveEntityConstantly>
        Script;
}