#pragma once
#include <vector>
#include <variant>
#include "Game.h"
namespace Core
{

    /// anchor - script type enum class
    enum class ScriptType
    {
        MoveEntitiesConstantly // move entities in constant speed
    };

    /// anchor - Script Bass class
    /// \brief script can change everything in the game
    /// \note when construct, pass the reference of current game state. Script
    /// knows the state and renderer knows the resources.
    class ScriptBase
    {
    private:
        ScriptType mtype;
        std::string mscriptName;

    public:
        // type
        ScriptType getType() { return this->mtype; }
        void setType(ScriptType type) { this->mtype = type; }
        // name
        void setScriptName(std::string &scriptName_ref) { mscriptName = scriptName_ref; }
        std::string getScriptName() { return mscriptName; }

        // every script has to have run
        virtual void run() = 0;

        /// \brief Construct a new Script Base object
        /// \param type
        /// \param state_ref
        ScriptBase(std::string &scriptName_ref, ScriptType type)
            : mscriptName(scriptName_ref), mtype(type) {}
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
        ScriptMoveEntityConstantly(std::string &scriptName_ref,
                                   std::vector<Entity *> targetList,
                                   glm::vec2 movement)
            : ScriptBase(scriptName_ref, ScriptType::MoveEntitiesConstantly),
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